#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <array>

#include "graphic_test.h"

struct Vertex
{
    glm::vec2 pos;

    static VkVertexInputBindingDescription GetBindingDesCription()
    {
        VkVertexInputBindingDescription bindingDesCription{};

        bindingDesCription.binding = 0; // the index in array of bindings
        bindingDesCription.stride = sizeof(Vertex);
        bindingDesCription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDesCription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescription{};
        
        attributeDescription[0].binding = 0;
        attributeDescription[0].location = 0;   // location in vertex shader
        attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[0].offset = offsetof(Vertex, pos);

        return attributeDescription;
    }
};

struct Instance
{
    glm::vec2 pos;
    float scale;
    glm::vec3 color;

    static VkVertexInputBindingDescription GetBindingDesCription()
    {
        VkVertexInputBindingDescription bindingDesCription{};

        bindingDesCription.binding = 1; // the index in array of bindings
        bindingDesCription.stride = sizeof(Instance);
        bindingDesCription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDesCription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescription{};
        
        attributeDescription[0].binding = 1;
        attributeDescription[0].location = 1;   // location in vertex shader
        attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[0].offset = offsetof(Instance, pos);

        attributeDescription[1].binding = 1;
        attributeDescription[1].location = 2;
        attributeDescription[1].format = VK_FORMAT_R32_SFLOAT;
        attributeDescription[1].offset = offsetof(Instance, scale);

        attributeDescription[2].binding = 1;
        attributeDescription[2].location = 3;
        attributeDescription[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[2].offset = offsetof(Instance, color);

        return attributeDescription;
    }
};

class VulkanTest : public GraphicTest
{
public:
    VulkanTest();
    ~VulkanTest();
    virtual void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) final;
    virtual void CreateRenderPass() override;
    virtual void CreateGraphicsPipeline() final;

public:
    HVK::BufferWarp m_vertexBuferWarp;
    HVK::BufferWarp m_instanceBufferWarp;
};