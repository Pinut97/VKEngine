#include "DepthBuffer.h"
#include "CommandPool.h"
#include "VulkanDevice.h"
#include "Image.h"
#include "ImageView.h"
#include "DeviceMemory.h"

VkFormat findSupportedFormat(const VulkanDevice& device, const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : formats) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(device.GPU(), format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
			return format;
	}
	throw std::runtime_error("Failed to find a suitable format!");
}

VkFormat findDepthFormat(const VulkanDevice& device)
{
	return findSupportedFormat(
		device, 
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL, 
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

DepthBuffer::DepthBuffer(CommandPool& commandPool, VkExtent2D extent) :
	format_(findDepthFormat(commandPool.Device()))
{
	image_ = std::unique_ptr<Image>(new Image(commandPool.Device(), extent, format_, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
	memory_ = std::unique_ptr<DeviceMemory>(new DeviceMemory(image_->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)));
	imageView_ = std::unique_ptr<class ImageView>(new class ImageView(commandPool.Device(), image_->Handle(), format_, VK_IMAGE_ASPECT_DEPTH_BIT));

	image_->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

DepthBuffer::~DepthBuffer()
{
	imageView_.reset();
	image_.reset();
	memory_.reset();
}