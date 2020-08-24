#pragma once
#include "headers.h"

class Texture final
{
public:

	static Texture loadTexture(const std::string& filename);

	Texture& operator = (const Texture&) = delete;
	Texture& operator = (Texture&&) = delete;

	Texture() = default;
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;
	~Texture() = default;

	const unsigned char* Pixels() const { return pixels_.get(); }
	int Width() const { return width_; }
	int Height() const { return height_; }

private:

	Texture(int width, int height, int channels, unsigned char* pixels);

	int width_;
	int height_;
	int channels_;
	std::unique_ptr<unsigned char, void (*) (void*)> pixels_;
};