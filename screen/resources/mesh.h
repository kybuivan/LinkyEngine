#ifndef MESH_H
#define MESH_H
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/gl.h>
#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh
{
public:

	Mesh() { glGenVertexArrays(1, & mVertexArray); }
	~Mesh() { glDeleteVertexArrays(1, & mVertexArray); }

	Mesh(std::string const & filename);
	Mesh(std::vector<Vertex> const & vertices,
			std::vector<GLuint> const & indices,
			std::map<GLuint, std::string> const & textures);

	void draw(GLuint shader);

private:
	Mesh(Mesh const &) = delete;
	Mesh & operator=(Mesh const &) = delete;

	void parse(std::string const & path, aiNode const * node, aiScene const * scene);
	void parse(std::string const & path, aiMesh const * mesh, aiScene const * scene);
	std::map<GLuint, std::string> process(std::string const & path,
											aiMaterial * material,
											aiTextureType type);

private:
	std::vector<std::unique_ptr<Mesh>> mSubMeshes;
	std::vector<GLuint> mIndices;
	std::vector<Vertex> mVertices;
	std::map<GLuint, std::string> mTextures;

	GLuint mVertexArray;
	GLuint mVertexBuffer;
	GLuint mElementBuffer;

};
#endif