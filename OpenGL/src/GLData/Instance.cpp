#define PRINT_MATRICES false

#include "Instance.h"
#include "ext/matrix_transform.hpp"
#include <iostream>
#include <gtx/string_cast.hpp>
#include "gtc/matrix_inverse.hpp"

spark::Instance::Instance(std::shared_ptr<Model> object_model, glm::vec3 position)
	: objModel(object_model), name(name_), mMatrix(1.0f), nMatrix(1.0f), displacement(1.0f), rotation(1.0f), scaling(1.0f)
{
	trans.vDisplacement.vector = position;
	trans.vRotation.vector = glm::vec3(0.0f);
	trans.vScale.vector = glm::vec3(1.0f);

	trans.vDisplacement.isUpdated = true;
	trans.vRotation.isUpdated = false;
	trans.vScale.isUpdated = false;

	updateModelMatrix();
}

glm::mat4 spark::Instance::getModelMatrix()
{
	updateModelMatrix();
	return mMatrix;
}


glm::mat4 spark::Instance::getTransMatrix()
{
	updateModelMatrix();
	return displacement;
}

glm::mat4 spark::Instance::getNormalMatrix()
{
	updateModelMatrix();
	return nMatrix;
}

void spark::Instance::updateModelMatrix()
{
	if (trans.vScale.isUpdated || trans.vRotation.isUpdated || trans.vDisplacement.isUpdated)
	{
		if(trans.vDisplacement.isUpdated)
		{
			displacement = glm::translate(glm::identity<glm::mat4>(), trans.vDisplacement.vector);

			trans.vDisplacement.isUpdated = false;

		}

		if (trans.vRotation.isUpdated)
		{
			rotation = glm::rotate(glm::identity<glm::mat4>(), glm::radians(trans.vRotation.vector.x), glm::vec3(1.0f, 0.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(trans.vRotation.vector.y), glm::vec3(0.0f, 1.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(trans.vRotation.vector.z), glm::vec3(0.0f, 0.0f, 1.0f));

			trans.vRotation.isUpdated = false;

		}

		if (trans.vScale.isUpdated)
		{
			scaling = glm::scale(glm::identity<glm::mat4>(), trans.vScale.vector);

			trans.vScale.isUpdated = false;

		}

		mMatrix = displacement * (rotation * scaling);
		nMatrix = glm::mat4(glm::inverseTranspose(glm::mat3(mMatrix)));

		
#if defined(_DEBUG) && PRINT_MATRICES
		std::cout << glm::to_string(rotation) << std::endl;
		std::cout << glm::to_string(mMatrix) << std::endl;
		std::cout << glm::to_string(nMatrix) << std::endl;
		std::cout << std::endl;
#endif // _DEBUG
	}
}

void spark::Instance::Move(const glm::vec3& dVec)
{
	trans.vDisplacement.vector += dVec;
	trans.vDisplacement.isUpdated = true;
}

void spark::Instance::Rotate(const glm::vec3& rVec)
{
	trans.vRotation.vector += rVec;
	trans.vRotation.isUpdated = true;
}

void spark::Instance::Scale(const glm::vec3& sVec)
{
	trans.vScale.vector *= sVec;
	trans.vScale.isUpdated = true;
}

void spark::Instance::Draw() const
{

	glDrawElements(GL_TRIANGLES, (GLsizei)objModel->getIndexSize(), 
					GL_UNSIGNED_INT, nullptr);
}

void spark::Instance::setPos(const glm::vec3& dVec)
{
	trans.vDisplacement.vector = dVec;
	trans.vDisplacement.isUpdated = true;
}

void spark::Instance::setRot(glm::vec3 const& rVec)
{
	trans.vRotation.vector = rVec;
	trans.vRotation.isUpdated = true;
}

void spark::Instance::setScale(glm::vec3 const& sVec)
{
	trans.vScale.vector = sVec;
	trans.vScale.isUpdated = true;
}


