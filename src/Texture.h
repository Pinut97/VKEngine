#pragma once
#include "headers.h"

class Texture {
public:

	static Texture loadTexture(const std::string& filename);

	Texture() = default;
	~Texture() = default;

	const unsigned char* Pixels() const { return pixels_.get(); }
	int Width() const { return width_; }
	int Height() const { return height_; }

private:

	Texture(int width, int height, int channels, unsigned char* pixels);

	int width_, height_, channels_;
	std::unique_ptr<unsigned char, void (*) (void*)> pixels_;
};