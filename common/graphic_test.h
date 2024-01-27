#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <array>

#include "buffer.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

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

struct Vertex;

class GraphicTest
{
public:
    void Run();
    GraphicTest(const uint32_t width = 800, const uint32_t height = 600);
    ~GraphicTest();

public:
    // main function
    void InitVulkan();
    void MainLoop();
    void CleanUp();
    void InitWindow();
    void CreateInstance();
    bool CheckValidationLayerSupport();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void SetupDebugMessenger();
    std::vector<const char*> GetInstanceRequiredExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    void PickPhysicalDevice();
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    void CreateSurface();
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void CreateSwapChain();
    void CreateImageViews();
    std::vector<char> ReadFile(const std::string &fileName);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);
    void CreateCommandPool();
    void CreateCommandBuffer();
    void DrawFrame();
    void CreateSyncObject();
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory& bufferMemory);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CreateHostToDeviceBuffer(const void *src_data, HVK::BufferWarp &bufferWarp);
    uint32_t FindMemoryType(uint32_t typeFillter, VkMemoryPropertyFlags properties);

public:
    virtual void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) = 0;
    virtual void CreateRenderPass() = 0;
    virtual void CreateGraphicsPipeline() = 0;

    virtual void CreateFramebuffers();
    virtual void CreateLogicalDevice();

public:
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

public:
    std::vector<const char *> m_deviceExtensions;
    std::vector<const char *> m_instanceExtensions;
    const uint32_t m_width;
    const uint32_t m_height;
};