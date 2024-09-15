
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>


#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"

int main() {

	Window::Init();
	
	Window window(640,480);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

	Mesh triangle;

	triangle.vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
	triangle.indices = {0,1,2};
	triangle.init();

	Shader shader;
	{
		ShaderModule vertexShader("../../src/shaders/vertex.vert",GL_VERTEX_SHADER);
		ShaderModule fragmentShader("../../src/shaders/fragment.frag",GL_FRAGMENT_SHADER);

		shader.init({&vertexShader, &fragmentShader});
	}
	while (!window.shouldClose()) {
		Window::PullEvents();

		window.clear();

		shader.bind();
		
		triangle.bind();
		triangle.draw();

		window.draw();
	}

	Window::Destroy();
	
	return 0;
}