/*
indirect draw. Draw type meshs in one drawcall. The first is a triangle with one triangle.
The second is a rect with two triangles. Set instance count of type meshes is two.
*/

#include "draw_indirect.h"
#include "tools.h"

#include <limits>
#include <algorithm>
#include <string>
#include <fstream>

// must use on vertex buffer
const std::vector<Vertex> vertices = 
{
    // triangle mesh
    {{0.0f, -0.5f}},
    {{0.5f, 0.5f}},
    {{-0.5f, 0.5f}},

    // rectangle mesh
    {{-0.5f, -0.5f}},
    {{0.5f, -0.5f}},
    {{0.5f, 0.5f}},
    {{-0.5f, -0.5f}},
    {{0.5f, 0.5f}},
    {{-0.5f, 0.5f}}
};

const std::vector<Instance> instances = 
{
    {{-0.5f, -0.5f}, {0.8f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.7f}, {0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.5f}, {0.0f, 1.0f, 1.0f}},
};

VulkanTest::VulkanTest()
{
    m_deviceFeatures.multiDrawIndirect = VK_TRUE;

    this->InitWindow();
    this->InitVulkan();  // must called in derived class, becase it has virtual function

    m_vertexBufferWarp.device = m_Device;
    m_vertexBufferWarp.bufferSize = vertices.size() * sizeof(Vertex);
    m_vertexBufferWarp.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    CreateHostToDeviceBuffer(vertices.data(), m_vertexBufferWarp);

    m_instanceBufferWarp.device = m_Device;
    m_instanceBufferWarp.bufferSize = instances.size() * sizeof(Instance);
    m_instanceBufferWarp.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    CreateHostToDeviceBuffer(instances.data(), m_instanceBufferWarp);

    PrepareIndirectData();
}

VulkanTest::~VulkanTest()
{
    m_vertexBufferWarp.Destroy();
    m_instanceBufferWarp.Destroy();
    m_indirectBufferWarp.Destroy();
}

void VulkanTest::CreateGraphicsPipeline()
{
    // shader
    auto vertShaderCode = ReadFile("../../draw_indirect/shaders/vert.spv");
    auto fragShaderCode = ReadFile("../../draw_indirect/shaders/frag.spv");

    VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // vertex description
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    Vertex::GetBindingDesCription(0, bindingDescriptions);
    Instance::GetBindingDesCription(1, bindingDescriptions);
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    Vertex::GetAttributeDescription(0, attributeDescriptions);
    Instance::GetAttributeDescription(1, attributeDescriptions);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    // input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // viewport
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.f;
    rasterizer.depthBiasSlopeFactor = 0.f;

    // multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // color blending 
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // pipeline layout
    // for uniform value in shaders
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    VK_CHECK_RESULT(vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

    // dynamic state
    // state can actually change without recreating the pipeline
    std::vector<VkDynamicState> dynamicStates = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_PipelineLayout;
    pipelineInfo.renderPass = m_RenderPass;
    pipelineInfo.subpass = 0;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline));

    vkDestroyShaderModule(m_Device, fragShaderModule, nullptr);
    vkDestroyShaderModule(m_Device, vertShaderModule, nullptr);
}


void VulkanTest::CreateRenderPass()
{
    // attachment description
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_SwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // subpass
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // dependency
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // render pass
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    VK_CHECK_RESULT(vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass));
}

void VulkanTest::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    // begin render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_RenderPass;
    renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_SwapChainExtent;
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // bind pipeline
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

    // bind vertex buffer
    {
        VkBuffer vertexBuffers[] = {m_vertexBufferWarp.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }
    // bind instance buffer
    {
        VkBuffer vertexBuffers[] = {m_instanceBufferWarp.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, vertexBuffers, offsets);
    }

    // set dynamic pipeline state
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_SwapChainExtent.width);
    viewport.height = static_cast<float>(m_SwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_SwapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // draw
    vkCmdDrawIndirect(commandBuffer, m_indirectBufferWarp.buffer, 0, 2, sizeof(VkDrawIndirectCommand));

    // finish
    vkCmdEndRenderPass(commandBuffer);
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
}

void VulkanTest::PrepareIndirectData()
{
    std::vector<VkDrawIndirectCommand> indirectCmds;

    indirectCmds.push_back({
        .vertexCount = 3,
        .instanceCount = 2,
        .firstVertex = 0,
        .firstInstance = 0
    });

    indirectCmds.push_back({
        .vertexCount = 6,
        .instanceCount = 2,
        .firstVertex = 3,
        .firstInstance = 2
    });

    m_indirectBufferWarp.device = m_Device;
    m_indirectBufferWarp.bufferSize = indirectCmds.size() * sizeof(VkDrawIndirectCommand);
    m_indirectBufferWarp.usage = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    CreateHostToDeviceBuffer(indirectCmds.data(), m_indirectBufferWarp);
}

int main()
{
    try
    {
        VulkanTest app;
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}