
#include "Instance.h"
#include "ext/matrix_transform.hpp"

Instance::Instance(Model* object_model, glm::vec3 position)
	: objModel(object_model), mMatrix(glm::mat4(1.0f))
{
	trans.vDisplacement.vector = position;
	trans.vRotation.vector = glm::vec3(0.0f);
	trans.vScale.vector = glm::vec3(1.0f);

	trans.vDisplacement.isUpdated = true;
	trans.vRotation.isUpdated = false;
	trans.vScale.isUpdated = false;

	updateModelMatrix();
}

Instance::~Instance()
{
}

Model* Instance::getModel()
{
	return objModel;
}

glm::mat4 Instance::getModelMatrix()
{
	updateModelMatrix();
	return mMatrix;
}

void Instance::updateModelMatrix()
{
	if(trans.vDisplacement.isUpdated)
	{
		mMatrix = glm::translate(mMatrix, trans.vDisplacement.vector);
		trans.vDisplacement.isUpdated = false;
	}

	if (trans.vRotation.isUpdated)
	{
		mMatrix = glm::rotate(mMatrix, trans.vRotation.vector.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mMatrix = glm::rotate(mMatrix, trans.vRotation.vector.y, glm::vec3(1.0f, 0.0f, 0.0f));
		mMatrix = glm::rotate(mMatrix, trans.vRotation.vector.z, glm::vec3(1.0f, 0.0f, 0.0f));

		trans.vRotation.isUpdated = false;
	}
	
	if (trans.vScale.isUpdated)
	{
		mMatrix = glm::scale(mMatrix, trans.vDisplacement.vector);
		trans.vScale.isUpdated = false;
	}
		
}



void Instance::Displace(const glm::vec3 dVec)
{
	trans.vDisplacement.vector += dVec;
	trans.vDisplacement.isUpdated = true;
}

void Instance::Rotate(const glm::vec3 rVec)
{
	trans.vRotation.vector += rVec;
	trans.vRotation.isUpdated = true;
}

void Instance::Scale(const glm::vec3 sVec)
{
	trans.vScale.vector *= sVec;
	trans.vScale.isUpdated = true;
}

void Instance::setDisplacement(const glm::vec3 dVec)
{

	trans.vDisplacement.vector = dVec;
	trans.vDisplacement.isUpdated = true;

}

void Instance::setRotation(const glm::vec3 rVec)
{
	trans.vDisplacement.vector = rVec;
	trans.vRotation.isUpdated = true;
}

void Instance::setScaling(const glm::vec3 sVec)
{
	trans.vDisplacement.vector = sVec;
	trans.vScale.isUpdated = true;
}
