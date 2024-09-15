
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>


#include "window.hpp"
#include "shader.hpp"

int main() {

	Window::Init();
	
	Window window(640,480);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

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

		window.draw();
	}

	Window::Destroy();
	
	return 0;
}