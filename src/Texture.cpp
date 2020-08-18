#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture Texture::loadTexture(const std::string& filename)
{
	std::cout << "Loading '" << filename << "'..." << std::flush;
	const auto timer = std::chrono::high_resolution_clock::now();

	// Load the texture in normal host memory.
	int width, height, channels;
	stbi_uc* pixels = stbi_load("textures/texture.jpg", &width, &height, &channels, STBI_rgb_alpha);
	VkDeviceSize imageSize = width * height * 4;

	if (!pixels)
		throw std::runtime_error("failed to load texture image!");

	const auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - timer).count();
	std::cout << "(" << width << " x " << height << " x " << channels << ") ";
	std::cout << elapsed << "s" << std::endl;

	return Texture(width, height, channels, pixels);

}

Texture::Texture(int width, int height, int channels, unsigned char* const pixels) :
	width_(width),
	height_(height),
	channels_(channels),
	pixels_(pixels, stbi_image_free)
{

}