
#include "ImageView.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"

ImageView::ImageView(const class VulkanDevice& device, const VkImage image, const VkFormat format, const VkImageAspectFlags aspectFlags) :
	device_(device), 
	image_(image), 
	format_(format)
{

	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.layerCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;

	if (vkCreateImageView(device.Handle(), &createInfo, nullptr, &imageView_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create image view!");

}

ImageView::~ImageView()
{
	//vkDestroyImageView(VulkanApplication::Instance()->DeviceObj()->Handle(), imageView_, nullptr);
}