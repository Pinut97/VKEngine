
#include "Scene.h"
#include "Buffer.h"
#include "Vertex.h"
#include "CommandPool.h"


void createDeviceBuffer(
	CommandPool& commandPool,
	const VkBufferUsageFlags usage,
	const std::vector<Vertex>& vertices,
	std::unique_ptr<Buffer>& buffer,
	std::unique_ptr<DeviceMemory>& memory)
{
	const auto& device = commandPool.Device();
	const auto contentSize = sizeof(vertices[0]) * vertices.size();

	buffer = std::unique_ptr<Buffer>(new Buffer(device, contentSize, usage));
	memory = std::unique_ptr<DeviceMemory>(new DeviceMemory(buffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));

	void* data = memory->map(0, contentSize);
	std::memcpy(data, vertices.data(), contentSize);
	memory->unmap();
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