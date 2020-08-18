#include "Image.h"
#include "VulkanDevice.h"
#include "CommandBuffers.h"
#include "Buffer.h"
#include "HelperFunctions.h"
#include "CommandPool.h"

Image::Image(const VulkanDevice& device, VkExtent2D extent, VkFormat format) :
	Image(device, extent, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
{
}

Image::Image(const VulkanDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage) :
	device_(device),
	extent_(extent),
	format_(format),
	imageLayout_(VK_IMAGE_LAYOUT_UNDEFINED)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType		= VK_IMAGE_TYPE_2D;
	imageInfo.extent.width	= extent.width;
	imageInfo.extent.height = extent.height;
	imageInfo.extent.depth	= 1;
	imageInfo.format		= format;
	imageInfo.mipLevels		= 1;
	imageInfo.arrayLayers	= 1;
	imageInfo.tiling		= tiling;
	imageInfo.initialLayout = imageLayout_;
	imageInfo.usage			= usage;
	imageInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples		= VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags			= 0;

	if (vkCreateImage(device.Handle(), &imageInfo, nullptr, &image_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create image!");
}

Image::~Image()
{
	if (image_ != nullptr) {
		vkDestroyImage(device_.Handle(), image_, nullptr);
		image_ = nullptr;
	}
}

VkMemoryRequirements Image::getMemoryRequirements() const
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device_.Handle(), image_, &memRequirements);
	return memRequirements;
}

DeviceMemory Image::allocateMemory(const VkMemoryPropertyFlags properties) const
{
	const auto requirements = getMemoryRequirements();
	DeviceMemory memory(device_, requirements.size, requirements.memoryTypeBits, properties);

	if (vkBindImageMemory(device_.Handle(), image_, memory.Handle(), 0) != VK_SUCCESS)
		throw std::runtime_error("Failed to bind image memory!");
}

void Image::transitionImageLayout(CommandPool& commandPool, VkImageLayout newLayout)
{
	CommandBuffers commandBuffers(commandPool, 1);
	commandBuffers[0] = beginSingleTimeCommands(commandPool);

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout						= imageLayout_;
	barrier.newLayout						= newLayout;
	barrier.srcQueueFamilyIndex				= VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex				= VK_QUEUE_FAMILY_IGNORED;
	barrier.image							= image_;
	barrier.subresourceRange.baseMipLevel	= 0;
	barrier.subresourceRange.levelCount		= 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount		= 1;

	if (imageLayout_ == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (imageLayout_ == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
		throw std::invalid_argument("unsupported layout transition!");

	vkCmdPipelineBarrier(commandBuffers[0], 0, 0, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	const auto graphicsQueue = commandPool.Device().GraphicsQueue();
	endSingleTimeCommand(commandBuffers[0], graphicsQueue, commandPool);

	imageLayout_ = newLayout;
}

void Image::copyFrom(CommandPool& commandPool, const Buffer& buffer)
{
	CommandBuffers commandBuffers(commandPool, 1);
	commandBuffers[0] = beginSingleTimeCommands(commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset			= 0;
	region.bufferRowLength		= 0;
	region.bufferImageHeight	= 0;

	region.imageSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel		= 0;
	region.imageSubresource.baseArrayLayer	= 0;
	region.imageSubresource.layerCount		= 1;

	region.imageOffset = { 0,0,0 };
	region.imageExtent = {
		extent_.width,
		extent_.height,
		1
	};

	vkCmdCopyBufferToImage(commandBuffers[0], buffer.Handle(), image_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	const auto graphicsQueue = commandPool.Device().GraphicsQueue();
	endSingleTimeCommand(commandBuffers[0], graphicsQueue, commandPool);
}