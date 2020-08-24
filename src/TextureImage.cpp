#include "TextureImage.h"
#include "Texture.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "Image.h"
#include "ImageView.h"
#include "Sampler.h"

TextureImage::TextureImage(CommandPool& commandPool, const Texture& texture)
{
	const VkDeviceSize imageSize = texture.Width() * texture.Height() * 4;
	const auto& device = commandPool.Device();

	auto stagingBuffer = std::make_unique<Buffer>(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	const auto data = stagingBufferMemory.map(0, imageSize);
	std::memcpy(data, texture.Pixels(), imageSize);
	stagingBufferMemory.unmap();

	VkExtent2D extent = {
		static_cast<uint32_t>(texture.Width()),
		static_cast<uint32_t>(texture.Height()),
	};

	image_ = std::unique_ptr<Image>(new Image(device, extent, VK_FORMAT_R8G8B8A8_UNORM));
	imageMemory_ = std::unique_ptr<DeviceMemory>(new DeviceMemory(image_->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)));
	imageView_ = std::unique_ptr<class ImageView>(new class ImageView(device, image_->Handle(), image_->Format(), VK_IMAGE_ASPECT_COLOR_BIT));
	sampler_ = std::unique_ptr<class Sampler>(new class Sampler(device, SamplerConfig()));

	image_->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	image_->copyFrom(commandPool, *stagingBuffer);
	image_->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

}

TextureImage::~TextureImage()
{
	image_.reset();
	imageMemory_.reset();
}