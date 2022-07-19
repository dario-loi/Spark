#include "Camera.h"
#include "ext/matrix_transform.hpp"
#include <ext/matrix_clip_space.hpp>

Camera::Camera( const float  FOV, float const aspect, float speed)
	: pos(0.0f, 0.0f, 2.0f), viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), nextMove(0.0f), 
	rotation(-90.0f,0.0f), speedWalk(speed), speedSprint(speed*1.44f), currSpeed(speed), isViewInvalidated(false), FOV(FOV), aspect(aspect)
{
	view = glm::lookAt(pos,pos + viewDirection, up);
	proj = glm::perspective(
		glm::radians(FOV), 
		4.0f / 3.0f,       
		0.1f,              
		100.0f             
	);
}

Camera::~Camera() = default;

glm::mat4 Camera::getView(float delta)
{
	UpdateView(delta);
	return view;
}

void Camera::setSpeed(float speed)
{
	this->currSpeed = speed;
}

glm::vec3 Camera::getSideVector() const
{

	return glm::normalize(glm::cross(viewDirection, up));
}

void Camera::setFOV(const float newFOV)
{
	FOV = newFOV;
}

void Camera::UpdateView(float delta)
{
	if (isViewInvalidated)
	{
		nextMove = nextMove * delta * currSpeed;

		pos += nextMove;

		nextMove = glm::vec3(0.0f);

		view = glm::lookAt(pos, pos + viewDirection, up);
		isViewInvalidated = false;
	}

}

void Camera::Move(glm::vec3 direction)
{

	isViewInvalidated = true;
	nextMove += direction;
}

void Camera::Move(float x, float y, float z)
{
	isViewInvalidated = true;
	nextMove += glm::vec3(x,y,z);
}

void Camera::Rotate(glm::vec2 rot)
{
	rotation += rot;

	if (rotation.y > 89.0f)
		rotation.y = 89.0f;
	if (rotation.y < -89.0f)
		rotation.y = -89.0f;


	viewDirection = glm::vec3(cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)), sin(glm::radians(rotation.y)), sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)));

	isViewInvalidated = true;
}

void Camera::Rotate(float yaw, float pitch)
{
	rotation += glm::vec2(yaw, pitch);

	if (rotation.y > 89.0f)
		rotation.y = 89.0f;
	if (rotation.y < -89.0f)
		rotation.y = -89.0f;
	

	viewDirection = glm::vec3(cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)), sin(glm::radians(rotation.y)), sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)));

	isViewInvalidated = true;
}

void Camera::Sprint(bool shouldSprint)
{
	currSpeed = shouldSprint ? speedSprint : speedWalk;
}
