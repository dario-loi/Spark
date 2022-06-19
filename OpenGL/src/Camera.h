#pragma once
#include "mat4x4.hpp"


class Camera
{

private:

	glm::mat4 view;
	glm::mat4 proj;

	glm::vec3 pos;
	glm::vec3 viewDirection;
	glm::vec3 up;

	glm::vec3 nextMove;
	glm::vec2 rotation;

	float speedWalk;
	float speedSprint;
	float currSpeed;

	bool isViewInvalidated;

	float FOV;
	const float aspect;



public:
	Camera(float  FOV, float const aspect, float speed);
   ~Camera();

	glm::mat4 getView(float delta);
	glm::mat4 getProj() const { return proj; }
	glm::vec3 getPos() const { return pos; }
	float getSpeed() const { return currSpeed; }
	void  setSpeed(float speed);

	glm::vec3 getSideVector() const;
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getUpDirection() const { return up; }

	float getFOV() const { return FOV; }
	void setFOV( float newFOV);

	void UpdateView(float delta);

	void Move( glm::vec3 direction);
	void Move( float x,  float y,  float z);

	void Rotate( glm::vec2 rotation);
	void Rotate( float yaw,  float pitch);

	void Sprint(bool);

};

