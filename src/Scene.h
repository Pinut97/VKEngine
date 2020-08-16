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
	const Buffer& VertexBuffer() const { return *vertexBuffer_; }

private:

	class CommandPool& commandPool_;
	std::vector<Vertex> vertices;

	std::unique_ptr<Buffer> vertexBuffer_;
	std::unique_ptr<DeviceMemory> vertexBufferMemory_;

	std::unique_ptr<Buffer> indexBuffer_;
	std::unique_ptr<DeviceMemory> indexBufferMemory_;
};
