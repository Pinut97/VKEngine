#pragma once
#include "headers.h"

class Buffer;
class Vertex;

class Scene final{
public:
	
	Scene(const Vertex& model);
	~Scene();

private:

	std::unique_ptr<Buffer> vertexBuffer_;
	const Vertex& model_;
};
