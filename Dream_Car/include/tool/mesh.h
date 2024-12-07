#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tool/shader.h>

#include <string>
#include <vector>

using namespace std;

#ifndef BUFFER_GROMETRY
struct Vertex
{
	glm::vec3 Position;	 // 顶点属性
	glm::vec3 Normal;		 // 法线
	glm::vec2 TexCoords; // 纹理坐标

	// 切线空间属性
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
#endif

class Mesh
{
public:
	// mesh Data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}
	// render the mesh
	void Draw(Shader &shader)
	{

		// draw mesh
		printf("vertices[0].Position: %f, %f, %f\n", vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

private:
	// render data
	unsigned int VBO, EBO;

	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};

#endif