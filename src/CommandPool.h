#pragma once
#include "headers.h"

class VulkanDevice;

class CommandPool {
public:
	CommandPool(const class VulkanDevice& device, uint32_t queueFamilyIndex, bool allowReset);
	~CommandPool();

	const VkCommandPool Handle() const{ return commandPool_; }
	const class VulkanDevice& Device() const { return device_; }

private:
	VkCommandPool commandPool_;
	const class VulkanDevice& device_;
};