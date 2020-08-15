#pragma once
#include "headers.h"

class VulkanDevice;

class DeviceMemory final
{
public:

	DeviceMemory(const VulkanDevice& device, size_t size, uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
	DeviceMemory(DeviceMemory&& other) noexcept;
	~DeviceMemory();

	void* map(size_t offset, size_t size);
	void unmap();

	const class VulkanDevice& Device() const { return device_; }
	VkDeviceMemory Handle() const { return memory_; }

private:

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

	const VulkanDevice& device_;
	VkDeviceMemory memory_;

};