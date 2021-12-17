#pragma once
#include "mat4x4.hpp"


class Camera
{

private:

	glm::mat4 view;
	glm::mat4 proj;

	glm::vec3 pos;

	float FOV;
	float aspect;



public:
	Camera(float const FOV, float const aspect);
	~Camera();

	glm::mat4 getView();
	glm::mat4 getProj();

	float getFOV();
	void setFOV(const float newFOV);

};

