#include "CommandPool.h"
#include "VulkanDevice.h"


CommandPool::CommandPool(const class VulkanDevice& device, const uint32_t queueFamilyIndex, const bool allowReset) :
	device_(device)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex	= queueFamilyIndex;
	poolInfo.flags				= allowReset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;

	if (vkCreateCommandPool(device.Handle(), &poolInfo, nullptr, &commandPool_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Command Pool!");
}

CommandPool::~CommandPool()
{
	if (commandPool_ != nullptr) {
		vkDestroyCommandPool(device_.Handle(), commandPool_, nullptr);
		commandPool_ = nullptr;
	}
}