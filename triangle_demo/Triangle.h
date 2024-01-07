#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <array>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily; // std::optional is a warpper that contains no value uitil you assign something to it
    std::optional<uint32_t> presentFamily;

    bool IsComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription GetBindingDesCription()
    {
        VkVertexInputBindingDescription bindingDesCription{};

        bindingDesCription.binding = 0; // the index in array of bindings
        bindingDesCription.stride = sizeof(Vertex);
        bindingDesCription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDesCription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescription{};
        
        attributeDescription[0].binding = 0;
        attributeDescription[0].location = 0;   // location in vertex shader
        attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[0].offset = offsetof(Vertex, pos);

        attributeDescription[1].binding = 0;
        attributeDescription[1].location = 1;
        attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[1].offset = offsetof(Vertex, color);

        return attributeDescription;
    }
};

class TriangleTest
{
public:
    void Run();

private:
    // main function
    void InitVulkan();
    void MainLoop();
    void CleanUp();

    // window
    void InitWindow();

    // instance
    void CreateInstance();

    // validation layers
    bool CheckValidationLayerSupport();

    // debug messager
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void SetupDebugMessenger();

    // extensions
    std::vector<const char*> GetRequiredExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

    // physical device
    void PickPhysicalDevice();
    bool IsDeviceSuitable(VkPhysicalDevice device);

    // queue families
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    // logical device and queue
    void CreateLogicalDevice();

    // surafce
    void CreateSurface();

    // swap chain
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void CreateSwapChain();

    // image views
    void CreateImageViews();

    // grraphics pipeline
    void CreateGraphicsPipeline();

    // shader
    std::vector<char> ReadFile(const std::string &fileName);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    // render pass
    void CreateRenderPass();

    // framebuffer
    void CreateFramebuffers();

    // command pool
    void CreateCommandPool();

    // command buffer
    void CreateCommandBuffer();
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    // draw
    void DrawFrame();

    // synchronization
    void CreateSyncObject();

    // vertex buffer
    void CreateVertexBuffer();

    // memory
    uint32_t FindMemoryType(uint32_t typeFillter, VkMemoryPropertyFlags properties);

private:
    GLFWwindow* m_Window;
    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device;
    VkQueue m_GraphicsQueue;
    VkSurfaceKHR m_Surface;
    VkQueue m_PresentQueue;
    VkSwapchainKHR m_SwapChain;
    std::vector<VkImage> m_SwapChainImages;
    VkFormat m_SwapChainImageFormat;
    VkExtent2D m_SwapChainExtent;
    std::vector<VkImageView> m_SwapChainImageViews;
    VkRenderPass m_RenderPass;
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;
    VkCommandPool m_CommandPool;
    VkCommandBuffer m_CommandBuffer;
    VkSemaphore m_ImageAvailableSemaphore;  // semaphore used to sync in GPU
    VkSemaphore m_RenderFinishedSemaphore;  
    VkFence m_InFlightFence;  // fence used to sync between GPU and CPU
    VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
};