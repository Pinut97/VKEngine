#pragma once
#include "headers.h"

class Buffer;
class DeviceMemory;
class Vertex;

class Scene final{
public:
	
	Scene(class CommandPool& commandPool);
	~Scene();


	//std::vector<Vertex> getVertices() { return vertices; }
	const std::vector<uint32_t> Indices() const { return indices; }

	const Buffer& VertexBuffer() const { return *vertexBuffer_; }
	const Buffer& IndexBuffer() const { return *indexBuffer_; }

private:

	class CommandPool& commandPool_;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::unique_ptr<Buffer> vertexBuffer_;
	std::unique_ptr<DeviceMemory> vertexBufferMemory_;

	std::unique_ptr<Buffer> indexBuffer_;
	std::unique_ptr<DeviceMemory> indexBufferMemory_;
};
