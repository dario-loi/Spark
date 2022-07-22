#pragma once
#include "Model.h"
#include "glm.hpp"
#include <memory>

class Instance
{
private: 
	std::shared_ptr<Model> objModel;

	glm::mat4 mMatrix;
	glm::mat4 nMatrix;

	glm::mat4 displacement;
	glm::mat4 rotation;
	glm::mat4 scaling;

	struct TransformElement
	{
		glm::vec3 vector;
		bool isUpdated;

	};

	struct Transform{
		TransformElement vDisplacement;
		TransformElement vRotation;
		TransformElement vScale;
	};

	Transform trans;

public:

	Instance(std::shared_ptr<Model> object_model, glm::vec3 position = glm::vec3(0.0f));

	std::shared_ptr<Model> getModel() const { return objModel; };
	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();
	glm::mat4 getTransMatrix();
	void updateModelMatrix();

	Transform getTransform() const { return trans; }

	void Move(glm::vec3 const& dVec);
	void Rotate(glm::vec3 const& rVec);
	void Scale(glm::vec3 const& sVec);

	void setPos(glm::vec3 const& dVec);
	void setRot(glm::vec3 const& rVec);
	void setScale(glm::vec3 const& sVec);

	void Draw() const;
};

