#include "Fence.h"
#include "VulkanDevice.h"

Fence::Fence(const class VulkanDevice& device, const bool signaled) :
	device_(device)
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	if (vkCreateFence(device.Handle(), &fenceInfo, nullptr, &fence_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create fence!");
}

Fence::~Fence()
{

}

void Fence::reset()
{
	if (vkResetFences(device_.Handle(), 1, &fence_) != VK_SUCCESS)
		throw std::runtime_error("Failed to reset fence!");
}

void Fence::wait(const uint64_t timeout) const
{
	if (vkWaitForFences(device_.Handle(), 1, &fence_, VK_TRUE, timeout) != VK_SUCCESS)
		throw std::runtime_error("Failed to wait for fence!");
}