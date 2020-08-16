
#include "Scene.h"
#include "Buffer.h"
#include "Vertex.h"
#include "CommandPool.h"

void CopyFromStagingBuffer(CommandPool& commandPool, Buffer& dstBuffer, const std::vector<Vertex>& content)
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
	//stagingBuffer.reset();
}

void createDeviceBuffer(
	CommandPool& commandPool,
	const VkBufferUsageFlags usage,
	const std::vector<Vertex>& vertices,
	std::unique_ptr<Buffer>& buffer,
	std::unique_ptr<DeviceMemory>& memory)
{
	const auto& device = commandPool.Device();
	VkDeviceSize contentSize = sizeof(vertices[0]) * vertices.size();

	buffer = std::unique_ptr<Buffer>(new Buffer(device, contentSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
	memory = std::unique_ptr<DeviceMemory>(new DeviceMemory(buffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));

	CopyFromStagingBuffer(commandPool, *buffer, vertices);
}

Scene::Scene(class CommandPool& commandPool) : 
	commandPool_(commandPool)
{
	vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices, vertexBuffer_, vertexBufferMemory_);
	
}

Scene::~Scene()
{
	vertexBuffer_.reset();
	vertexBufferMemory_.reset();
}