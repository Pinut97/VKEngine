
#include "Scene.h"
#include "Vertex.h"
#include "CommandPool.h"
#include "Texture.h"
#include "TextureImage.h"
#include "Buffer.h"
#include "ImageView.h"
#include "Sampler.h"

template <class T>
void CopyFromStagingBuffer(CommandPool& commandPool, Buffer& dstBuffer, const std::vector<T>& content)
{
	const auto& device = commandPool.Device();
	const auto contentSize = sizeof(content[0]) * content.size();

	// Create a temporary host-visible staging buffer
	auto stagingBuffer = std::make_unique<Buffer>(device, contentSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy the host data into staging buffer
	const auto data = stagingBufferMemory.map(0, contentSize);
	std::memcpy(data, content.data(), contentSize);
	stagingBufferMemory.unmap();

	// Copy staging buffer to device buffer
	dstBuffer.copyFrom(commandPool, *stagingBuffer, contentSize);

	// Delete the buffer before the memory
	stagingBuffer.reset();
}

template <class T>
void createDeviceBuffer(
	CommandPool& commandPool,
	const VkBufferUsageFlags usage,
	const std::vector<T>& content,
	std::unique_ptr<Buffer>& buffer,
	std::unique_ptr<DeviceMemory>& memory)
{
	const auto& device = commandPool.Device();
	VkDeviceSize contentSize = sizeof(content[0]) * content.size();

	buffer = std::unique_ptr<Buffer>(new Buffer(device, contentSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
	memory = std::unique_ptr<DeviceMemory>(new DeviceMemory(buffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));

	CopyFromStagingBuffer(commandPool, *buffer, content);
}

Scene::Scene(class CommandPool& commandPool) : 
	commandPool_(commandPool)
{
	vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	indices = { 0, 1, 2, 2, 3, 0 };

	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices, vertexBuffer_, vertexBufferMemory_);
	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indices, indexBuffer_, indexBufferMemory_);

	//textures_.push_back(Texture::loadTexture("../../data/textures/sanFrancisco.jpg"));

	std::vector<Texture> textures;
	textures.push_back(Texture::loadTexture("../../data/textures/sanFrancisco.jpg"));

	textureImages_.reserve(textures.size());
	textureImageViewHandles_.resize(textures.size());
	textureSamplerHandles_.resize(textures.size());
	for (size_t i = 0; i < textures.size(); i++) {
		textureImages_.emplace_back(new TextureImage(commandPool, textures[i]));
		textureImageViewHandles_[i] = textureImages_[i]->ImageView().Handle();
		textureSamplerHandles_[i] = textureImages_[i]->Sampler().Handle();
	}
	//textureImages_.push_back(std::unique_ptr<TextureImage>(new TextureImage(commandPool, Texture::loadTexture("../../data/textures/sanFrancisco.jpg"))));

}

Scene::~Scene()
{
	vertexBuffer_.reset();
	vertexBufferMemory_.reset();
}