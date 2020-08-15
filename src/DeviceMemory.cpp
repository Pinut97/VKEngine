#include "DeviceMemory.h"
#include "VulkanDevice.h"

DeviceMemory::DeviceMemory(
	const class VulkanDevice& device, 
	const size_t size, 
	const uint32_t memoryTypeBits, 
	const VkMemoryPropertyFlags properties) :
	device_(device)
{
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = findMemoryType(memoryTypeBits, properties);

	if (vkAllocateMemory(device_.Handle(), &allocInfo, nullptr, &memory_) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate memory!");
}

DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept :
	device_(other.device_),
	memory_(other.memory_)
{
	other.memory_ = nullptr;
}

DeviceMemory::~DeviceMemory()
{
	if (memory_ != nullptr) {
		vkFreeMemory(device_.Handle(), memory_, nullptr);
		memory_ = nullptr;
	}
}

uint32_t DeviceMemory::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device_.GPU(), &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

void* DeviceMemory::map(size_t offset, size_t size)
{
	void* data;
	if (vkMapMemory(device_.Handle(), memory_, offset, size, 0, &data) != VK_SUCCESS)
		throw std::runtime_error("Failed to map memory!");

	return data;
}

void DeviceMemory::unmap()
{
	vkUnmapMemory(device_.Handle(), memory_);
}