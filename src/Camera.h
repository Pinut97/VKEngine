#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

const float SPEED = 1.0f;

class Camera final
{
public:

	//Camera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f), 
	//	glm::vec3 center = glm::vec3(0.0f, 0.0f, -1.0f),
	//	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	Camera();
	~Camera();

	//camera attributes
	glm::vec3 eye_;
	glm::vec3 center_;
	glm::vec3 up_;

	//float rotationSpeed_ =  1.0f;
	float movementSpeed_;

	void setOrthographic(float left, float right, float bottom, float top, float near, float far);
	void setPerspective(float fov, float aspect, float near, float far);
	void update(float dt);
	void move(glm::vec3 movement);

	glm::mat4 Projection() { return glm::perspective(glm::radians(45.0f), aspect_, znear, zfar); }
	glm::mat4 View() { return glm::lookAt(eye_, eye_ + center_, up_); }

private:

	enum type_ {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	struct {
		glm::mat4 projection;
		glm::mat4 view;
	}matrices_;

	// orthographic values
	float left_, right_, bottom_, top_;

	float fov_;
	float aspect_;
	float znear, zfar;

};