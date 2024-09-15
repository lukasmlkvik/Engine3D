
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>


#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"


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

	Camera camera;
	camera.setPerspectiveProjection(glm::radians(50.f), 0.75f, 0.1f, 500.f);
	camera.transform.position = {0,0,-2};


	auto currentTime = std::chrono::high_resolution_clock::now();

	ui32 shaderProjMatId = glGetUniformLocation(shader.shaderId, "projMat");
	while (!window.shouldClose()) {
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		auto currentTime = std::chrono::high_resolution_clock::now();

		Window::PullEvents();

		window.clear();

		shader.bind();

		camera.setViewYXZ();
		glUniformMatrix4fv(shaderProjMatId, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

		triangle.bind();
		triangle.draw();

		window.draw();
	}

	Window::Destroy();
	
	return 0;
}