#pragma once
#include "headers.h"
#include "DeviceMemory.h"

class Buffer;
class VulkanDevice;
class CommandPool;

class Image final
{
public:

	Image(const VulkanDevice& device, VkExtent2D extent, VkFormat format);
	Image(const VulkanDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);

	~Image();

	const VkImage& Handle() const { return image_; }
	const VulkanDevice& Device() const { return device_; }
	VkFormat Format() const { return format_; }
	VkExtent2D Extent() const { return extent_; }

	DeviceMemory allocateMemory(VkMemoryPropertyFlags properties) const;
	VkMemoryRequirements getMemoryRequirements() const;

	void transitionImageLayout(CommandPool& commandPool, VkImageLayout newLayout);
	void copyFrom(CommandPool& commandPool, const Buffer& buffer);

private:

	const VulkanDevice&	device_;
	const VkExtent2D&	extent_;
	const VkFormat&		format_;
	VkImageLayout		imageLayout_;
	VkImage				image_;

};