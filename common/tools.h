#include <iostream>
#include <string>
#include <vulkan/vulkan.h>

std::string errorString(VkResult errorCode);

// https://github.com/SaschaWillems/Vulkan.git
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << errorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << "\n"; \
		throw std::runtime_error("vulkan failed");																		\
	}																									\
}