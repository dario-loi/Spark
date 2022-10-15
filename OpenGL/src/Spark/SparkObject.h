#pragma once

#include "..\GLData\Instance.h"
#include "..\GLData\Texture.h"
#include "SparkConfig.h"
#include <string>
#include <memory>
#include "boost/uuid/uuid.hpp"

namespace spark
{

class SparkObject
{
public:

	SparkObject(const boost::uuids::uuid UUID, std::string const& name,
		std::shared_ptr<Model> const& instance_model)
		: UUID{ UUID }, name{ name }, flags{ spark::SparkRenderFlags::NONE }, 
			instance{ instance_model, glm::vec3(0.0F) }
	{

	}

	void setDiffuse(std::shared_ptr<spark::Texture> const& diff);
	void setSpecular(std::shared_ptr<spark::Texture> const& spec);	
	void setNormal(std::shared_ptr<spark::Texture> const& norm);
	void setEmissive(std::shared_ptr<spark::Texture> const& emi);

	[[nodiscard]] const boost::uuids::uuid getUUID() const { return UUID; }
	[[nodiscard]] std::string getModelName() const { return instance.getModelName(); }
	[[nodiscard]] Instance const& getInstance() const { return instance; }

private:

	/*
		Holds together a number of coefficients that represent
		this object's behaviour during the Rendering phase, usually
		related to the lightning model
	*/
	struct Properties
	{
		//Fallback solid color
		glm::vec4 color{ 0.0F, 0.0F, 0.0F, 1.0F };

		//Light Receiver properties
		float glossiness = 0.5F;	//Determines specular component intensity, in ]0, 1]

		//Light Transmitter properties
		float alpha = 0.0F;			//linear attenuation coefficient;
		float beta = 0.0F;			//quadratic attenuation coefficient;

	};

	struct Textures
	{
		std::shared_ptr<spark::Texture> diffuse;
		std::shared_ptr<spark::Texture> specular;
		std::shared_ptr<spark::Texture> normal;
		std::shared_ptr<spark::Texture> emissive;
	};

	const boost::uuids::uuid UUID;
	std::string name;
	
	spark::SparkRenderFlags flags;

	Textures textures;
	Properties properties;

	spark::Instance instance;



};

}