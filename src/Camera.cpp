#include "Camera.h"

/*
Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up) :
	eye_(eye),
	center_(center),
	up_(up)
	//movementSpeed_(SPEED)
{
	//glm::mat4 mat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
	//glm::mat4 viw = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	aspect_ = 800.0f / 600.0f;

	//matrices_.view = glm::lookAt(eye_, center_, up_);
}
*/

Camera::Camera() 
{
	eye_ = glm::vec3(20.0f, 20.0f, 20.0f);
	up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	center_ = glm::vec3(0.0f, 0.0f, 0.0f);

	aspect_ = 800.0f / 600.0f;
	movementSpeed_ = 1.0f;

	fov_ = glm::radians(45.0f);
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	//type_ = type_::ORTHOGRAPHIC;

	this->left_		= left;
	this->right_	= right;
	this->bottom_	= bottom;
	this->top_		= top;
	this->znear		= near;
	this->zfar		= far;

	matrices_.projection = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	this->fov_		= fov;
	this->aspect_	= aspect;
	this->znear		= near;
	this->zfar		= far;

	matrices_.projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::move(glm::vec3 movement)
{
	eye_ = eye_ + movement;
	center_ = center_ + movement;
}

void Camera::update(float dt)
{

}