#pragma once
#include "headers.h"

class VulkanDevice;

class Fence
{
public:
	Fence(const class VulkanDevice& device, bool signaled);
	~Fence();

	const class VulkanDevice& Device() const { return device_; }
	const VkFence& Handle() const { return fence_; }

	void reset();
	void wait(uint64_t timout) const;

private:

	const class VulkanDevice& device_;
	VkFence fence_;
};