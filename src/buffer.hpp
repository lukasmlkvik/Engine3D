#pragma once

#include<glad/glad.h>
#include<vector>

#define BUFFER_TYPE int
class Buffer
{
public:
	Buffer() {
		glGenBuffers(1, &ID);
	}
	template<class T>
	void init(std::vector<T>* vec, BUFFER_TYPE t) {
		type = t;
		glBindBuffer(t, ID);
		glBufferData(t, vec->size() * sizeof(T), vec->data(), GL_STATIC_DRAW);
	}
	~Buffer() {
		glDeleteBuffers(1, &ID);
	};

	void bind() {

		glBindBuffer(type, ID);
	}
	void unBind() {

		glBindBuffer(type, 0);
	}
private:
	GLuint ID;
	BUFFER_TYPE type;
};
