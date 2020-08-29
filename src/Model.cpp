#include "Model.h"
#include "Vertex.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj/tiny_obj_loader.h>

Model Model::loadModel(const std::string& filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

//	tinyobj::ObjReader obj;
//	if (!obj.ParseFromFile(filename))
//		throw std::runtime_error("Error: " + obj.Error());
	

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
		throw std::runtime_error("Failed to load model! Error: " + err);

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			//vertices.push_back(vertex);
			indices.push_back(indices.size());

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices.push_back(vertex);
		}
	}

	return Model(std::move(vertices), std::move(indices));
	
}

Model::Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices) :
	vertices_(vertices),
	indices_(indices)
{

}