#pragma once

#include "headers.h"

class VulkanDevice;

class ImageView final
{
public:
	ImageView(const VulkanDevice& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	~ImageView();

	const VkImageView Handle() const { return imageView_; }
	const VulkanDevice& Device() const { return device_; }

private:

	const class VulkanDevice&	device_;
	const VkImage				image_;
	const VkFormat				format_;
	VkImageView					imageView_;
};