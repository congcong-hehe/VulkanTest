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

    static void GetBindingDesCription(const uint32_t bindingIndex, std::vector<VkVertexInputBindingDescription> &bindingDesCriptions)
    {
        bindingDesCriptions.push_back({
            .binding = bindingIndex,
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        });
    }

    static void GetAttributeDescription(const uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions)
    {
        attributeDescriptions.push_back({
            .location = 0,
            .binding = bindingIndex,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(Vertex, pos)
        });
    }
};

struct Instance
{
    glm::vec2 pos;
    float scale;
    glm::vec3 color;

    static void GetBindingDesCription(const uint32_t bindingIndex, std::vector<VkVertexInputBindingDescription> &bindingDesCriptions)
    {
        bindingDesCriptions.push_back({
            .binding = bindingIndex,
            .stride = sizeof(Instance),
            .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE
        });
    }

    static void GetAttributeDescription(const uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions)
    {
        attributeDescriptions.push_back({
            .location = 1,
            .binding = bindingIndex,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(Instance, pos)
        });

        attributeDescriptions.push_back({
            .location = 2,
            .binding = bindingIndex,
            .format = VK_FORMAT_R32_SFLOAT,
            .offset = offsetof(Instance, scale)
        });

        attributeDescriptions.push_back({
            .location = 3,
            .binding = bindingIndex,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Instance, color)
        });
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
    void PrepareIndirectData();

public:
    HVK::BufferWarp m_vertexBufferWarp;
    HVK::BufferWarp m_instanceBufferWarp;
    HVK::BufferWarp m_indirectBufferWarp;
};