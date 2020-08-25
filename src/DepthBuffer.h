#pragma once

#include "headers.h"

class Image;
class DeviceMemory;
class ImageView;
class CommandPool;

class DepthBuffer final
{
public:
	DepthBuffer(CommandPool& commandPool, VkExtent2D extent);
	~DepthBuffer();

	const VkFormat Format() const { return format_; }
	const class ImageView& ImageView() const { return *imageView_; };

	static bool hasStencilComponent(const VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

private:

	VkFormat							format_;
	std::unique_ptr<Image>				image_;
	std::unique_ptr<DeviceMemory>		memory_;
	std::unique_ptr<class ImageView>	imageView_;
};