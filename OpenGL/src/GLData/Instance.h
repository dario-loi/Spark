#pragma once
#include "Model.h"
#include "glm.hpp"
#include <memory>

class Instance
{
private: 
	std::shared_ptr<Model> objModel;

	glm::mat4 mMatrix;

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
	void updateModelMatrix();

	Transform getTransform() const { return trans; }

	void Move(const glm::vec3& dVec);
	void Rotate(const glm::vec3& rVec);
	void Scale(const glm::vec3& sVec);

	void Draw() const;
};

