#pragma once
#include "headers.h"

class Vertex;

class Model final
{
public:

	static Model loadModel(const std::string& filename);

	Model() = default;
	~Model() = default;

	const std::vector<Vertex>& vertices() const { return vertices_; }
	const std::vector<uint32_t>& indices() const { return indices_; }

private:

	Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);

	std::vector<Vertex> vertices_;
	std::vector<uint32_t> indices_;
};