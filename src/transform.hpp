#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

struct TransformComponent {
	glm::vec3 position{0.f, 0.f, 0.f};
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation{0.f, 0.f, 0.f};


	glm::vec3 getForward() {
		return glm::vec3{ sin(rotation.y) * cos(rotation.x), sin(-rotation.x), cos(rotation.y) * cos(rotation.x) };
	}
	glm::vec3 getRight() {
		return glm::vec3{ cos(rotation.z) * cos(rotation.y),sin(-rotation.z) * cos(rotation.y),sin(-rotation.y) };
	}
	glm::vec3 getUp() {
		return glm::cross(getForward(), getRight());
	}

	glm::mat4 mat4() {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},
			{position.x, position.y, position.z, 1.0f} };
	}
	glm::mat3 normalMatrix() {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 invScale = 1.0f / scale;

		return glm::mat3{
			{
				invScale.x * (c1 * c3 + s1 * s2 * s3),
				invScale.x * (c2 * s3),
				invScale.x * (c1 * s2 * s3 - c3 * s1)
			},
			{
				invScale.y * (c3 * s1 * s2 - c1 * s3),
				invScale.y * (c2 * c3),
				invScale.y * (c1 * c3 * s2 + s1 * s3)
			},
			{
				invScale.z * (c2 * s1),
				invScale.z * (-s2),
				invScale.z * (c1 * c2)
			}
		};
	}
};
