#pragma once
#include "headers.h"

class CommandPool;
class Texture;
class Image;
class DeviceMemory;

class TextureImage {
public:

	TextureImage(CommandPool& commandPool, const Texture& texture);
	~TextureImage();

private:

	std::unique_ptr<Image>			image_;
	std::unique_ptr<DeviceMemory>	imageMemory_;
};