#include "Camera.h"
#include "ext/matrix_transform.hpp"
#include <ext/matrix_clip_space.hpp>

Camera::Camera( const float  FOV, float const aspect)
	: FOV(FOV), aspect(aspect), pos(glm::vec3(5.0f, 0.0f, -5.0f))
{
	view = glm::lookAt(pos, glm::vec3(pos.x, pos.y, -pos.z), glm::vec3(0, 1, 0));
	proj = glm::perspective(
		glm::radians(FOV), 
		4.0f / 3.0f,       
		0.1f,              
		100.0f             
	);
}

Camera::~Camera()
{
}

glm::mat4 Camera::getView()
{
	return view;
}

glm::mat4 Camera::getProj()
{
	return proj;
}

float Camera::getFOV()
{
	return FOV;
}

void Camera::setFOV(const float newFOV)
{
	FOV = newFOV;
}
