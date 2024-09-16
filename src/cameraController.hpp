#pragma once
#include "camera.hpp"


class CameraController
{
public:
    void onForward() {
        forward = true;
    }
    void onForwardStop() {
        forward = false;
    }

    void onBack() {
        back = true;
    }
    void onBackStop() {
        back = false;
    }

    void onLeft() {
        left = true;
    }
    void onLeftStop() {
        left = false;
    }

    void onRight() {
        right = true;
    }
    void onRightStop() {
        right = false;
    }

    void onUp() {
        up = true;
    }
    void onUpStop() {
        up = false;
    }

    void onDown() {
        down = true;
    }
    void onDownStop() {
        down = false;
    }



    void onTurn(float x, float y) {

        glm::vec2 dir = { x - lastPoz.x , y - lastPoz.y };

        glm::vec3 rotate{ 0 };
        rotate.x = dir.y;
        rotate.y = dir.x;


        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            camera->transform.rotation += mouseSeneitivity * glm::normalize(rotate);
        }


        camera->transform.rotation.x = glm::clamp(camera->transform.rotation.x, -glm::pi<float>() / 2, glm::pi<float>() / 2);
        camera->transform.rotation.y = glm::mod(camera->transform.rotation.y, glm::two_pi<float>());

        lastPoz.x = x;
        lastPoz.y = y;
    }


    void onZoomOut() {
        camera->setFovy(glm::max(camera->fovy + -0.1f, 0.02665f));
        mouseSeneitivity = camera->fovy * 10.f / 0.87f;
    }

    void onZoomIn() {
        camera->setFovy(glm::min(camera->fovy + 0.1f, 3.02665f));
        mouseSeneitivity = camera->fovy * 10.f / 0.87f;
    }

	void update(float dt) {

        float yaw = camera->transform.rotation.y;
        float xaw = camera->transform.rotation.x;
        float zaw = 0.f;

        const glm::vec3 forwardDir{ sin(yaw) * cos(xaw), sin(-xaw), cos(yaw) * cos(xaw) };
        const glm::vec3 upDir{ 0.f, 1.f, 0.f };

        const glm::vec3 rightDir{ cos(zaw) * cos(yaw),sin(-zaw) * cos(yaw),sin(-yaw) };


        glm::vec3 moveDir{ 0.f };
        if (forward) moveDir += forwardDir;
        if (back) moveDir -= forwardDir;
        if (right) moveDir += rightDir;
        if (left) moveDir -= rightDir;
        if (up) moveDir += upDir;
        if (down) moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            camera->transform.position += moveSpeed * dt * glm::normalize(moveDir);
        }
	}

	Camera* camera;
protected:

    float moveSpeed = 3.0f;
    float mouseSeneitivity = .01f;

    glm::vec2 lastPoz{ 0.0f };

    bool forward = false;
    bool back = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

};
