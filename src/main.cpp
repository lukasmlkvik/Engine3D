
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
#include "cameraController.hpp"
#include "sphere.hpp"


int main() {

	Window::Init();
	
	Window window(640,480);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

	Mesh triangle;
	Sphere shpere;
	shpere.create(10);

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

	CameraController cc;
	cc.camera = &camera;

	window.kc.registerEvent(GLFW_KEY_W, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onForward();}));
	window.kc.registerEvent(GLFW_KEY_W, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onForwardStop();}));

	window.kc.registerEvent(GLFW_KEY_S, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onBack();}));
	window.kc.registerEvent(GLFW_KEY_S, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onBackStop();}));

	window.kc.registerEvent(GLFW_KEY_D, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onRight();}));
	window.kc.registerEvent(GLFW_KEY_D, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onRightStop();}));

	window.kc.registerEvent(GLFW_KEY_A, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onLeft();}));
	window.kc.registerEvent(GLFW_KEY_A, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onLeftStop();}));

	window.kc.registerEvent(GLFW_KEY_UP, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onUp();}));
	window.kc.registerEvent(GLFW_KEY_UP, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onUpStop();}));

	window.kc.registerEvent(GLFW_KEY_DOWN, GLFW_PRESS, new EventController<CameraController>(&cc, [](CameraController* c) {c->onDown();}));
	window.kc.registerEvent(GLFW_KEY_DOWN, GLFW_RELEASE, new EventController<CameraController>(&cc, [](CameraController* c) {c->onDownStop();}));

	window.mc.registerEvent(new EventMouseMove<CameraController>(&cc, [](CameraController* c, float x, float y) {c->onTurn(x,y);}));

	window.kc.registerEvent(GLFW_KEY_ESCAPE, GLFW_PRESS, new EventController<Window>(&window, [](Window* w) {w->unLockCursor();}));
	window.kc.registerEvent(GLFW_KEY_TAB, GLFW_PRESS, new EventController<Window>(&window, [](Window* w) {w->lockCursor();}));


	auto currentTime = std::chrono::high_resolution_clock::now();

	ui32 shaderProjMatId = glGetUniformLocation(shader.shaderId, "projMat");

	ui32 frameCount = 0;
	float time = 0.0f;
	float fps = 0.0f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	while (!window.shouldClose()) {
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		frameCount++;
		time += frameTime;
		if(time>=.1f){
			fps = frameCount/time;
			time = 0.0f;
			frameCount = 0;
		}

		window.setTitle("Engine3D fps: " + std::to_string(fps));

		Window::PullEvents();

		window.clear();

		shader.bind();

		cc.update(frameTime);
		camera.setAspect(window.aspectRatio);
		camera.setViewYXZ();
		glUniformMatrix4fv(shaderProjMatId, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

		shpere.bind();
		shpere.draw();

		window.draw();
	}

	Window::Destroy();
	
	return 0;
}