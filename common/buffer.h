#include <vulkan/vulkan.h>

namespace HVK 
{
    
    struct BufferWarp
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDevice device;
        VkDeviceSize bufferSize = 0;
        VkBufferUsageFlagBits usage;

        void Destroy()
        {
            if (buffer)
            {
                vkDestroyBuffer(device, buffer, nullptr);
            }
            if (memory)
            {
                vkFreeMemory(device, memory, nullptr);
            }
        }
    };

} // namespace HVK