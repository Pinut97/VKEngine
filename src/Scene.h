#pragma once
#include "headers.h"
//#include "Buffer.h"

class DeviceMemory;
struct Vertex;
class Texture;
class TextureImage;
class Buffer;
class Model;

class Scene final {
public:

	Scene(class CommandPool& commandPool);
	~Scene();

	//std::vector<Vertex> getVertices() { return vertices; }
	const std::vector<uint32_t> Indices() const { return indices; }

	const Buffer& VertexBuffer() const { return *vertexBuffer_; }
	const Buffer& IndexBuffer() const { return *indexBuffer_; }
	const std::vector<VkImageView>& ImageViewHandles() const { return textureImageViewHandles_; }
	const std::vector<VkSampler>& TextureSamplers() const { return textureSamplerHandles_; }

private:

	class CommandPool& commandPool_;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::unique_ptr<Buffer> vertexBuffer_;
	std::unique_ptr<DeviceMemory> vertexBufferMemory_;

	std::unique_ptr<Buffer> indexBuffer_;
	std::unique_ptr<DeviceMemory> indexBufferMemory_;

	std::vector<Model> models_;

	//std::vector<Texture> textures_;
	std::vector<VkImageView> textureImageViewHandles_;
	std::vector<std::unique_ptr<TextureImage>> textureImages_;
	std::vector<VkSampler> textureSamplerHandles_;
};
