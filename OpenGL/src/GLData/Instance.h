#pragma once
#include "Model.h"
#include "glm.hpp"

class Instance
{
private: 
	Model* objModel;

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

	Instance(Model* object_model, glm::vec3 position = glm::vec3(0.0f));
	~Instance();

	Model* getModel();
	glm::mat4 getModelMatrix();
	void updateModelMatrix();

	void Displace(const glm::vec3 dVec);
	void Rotate(const glm::vec3 rVec);
	void Scale(const glm::vec3 sVec);

	void setDisplacement(const glm::vec3 dVec);
	void setRotation(const glm::vec3 rVec);
	void setScaling(const glm::vec3 sVec);
};

