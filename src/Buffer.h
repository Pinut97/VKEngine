#pragma once
#include "headers.h"
#include "DeviceMemory.h"
#include "VulkanDevice.h"

class CommandPool;

class Buffer
{
public:
	Buffer(const class VulkanDevice& device, size_t size, VkBufferUsageFlags usage);
	~Buffer();

	const VulkanDevice& Device() const { return device_; }

	DeviceMemory allocateMemory(const VkMemoryPropertyFlags properties);
	VkMemoryRequirements getMemoryRequirements() const;
	void copyFrom(CommandPool& commandPool, const Buffer src, VkDeviceSize size);

	const VkBuffer Handle() const { return buffer_; }
	const size_t Size() const { return size_; }

private:

	VkBuffer					buffer_;
	const class VulkanDevice&	device_;
	const size_t				size_;
};