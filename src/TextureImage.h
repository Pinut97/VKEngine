#pragma once
#include "headers.h"

class CommandPool;
class Texture;
class Image;
class DeviceMemory;
class ImageView;
class Sampler;

class TextureImage {
public:

	TextureImage(CommandPool& commandPool, const Texture& texture);
	~TextureImage();

	const ImageView& ImageView() const { return *imageView_; }
	const Sampler& Sampler() const { return *sampler_; }

private:

	std::unique_ptr<class Image>		image_;
	std::unique_ptr<class DeviceMemory>	imageMemory_;
	std::unique_ptr<class ImageView>	imageView_;
	std::unique_ptr<class Sampler>		sampler_;
};