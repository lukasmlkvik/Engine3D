#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vector>

#include "types.hpp"
#include "buffer.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};


class Mesh
{
public:
    
	GLuint ID;
	Buffer vertexBuffer;
	Buffer indexBuffer;
	std::vector<Vertex> vertices;
	std::vector<ui32> indices;

	Mesh()  { 
		glGenVertexArrays(1, &ID);
	}
	~Mesh() {
		glDeleteVertexArrays(1, &ID);
	}

	void init() {
		bind();
		vertexBuffer.init(&vertices, GL_ARRAY_BUFFER);
		indexBuffer.init(&indices, GL_ELEMENT_ARRAY_BUFFER);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

		unBind();
		vertexBuffer.unBind();
		indexBuffer.unBind();
	}

	void bind() {
		glBindVertexArray(ID);
	}
	void unBind() {
		glBindVertexArray(0);
	}
	void draw () {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};


