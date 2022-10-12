#pragma once

#include "..\GLData\Instance.h"
#include "..\GLData\Texture.h"
#include "SparkConfig.h"
#include <vector>
#include <string>

namespace spark
{

class SparkObject
{
public:

	SparkObject();
	void render();


private:

	/*
		Holds together a number of coefficients that represent
		this object's behaviour during the Rendering phase, usually
		related to the lightning model
	*/
	struct Properties
	{
		//Light Receiver properties
		float glossiness = 0.5f;	//Determines specular component intensity, in ]0, 1]

		//Light Transmitter properties
		float alpha = 0.0f;			//linear attenuation coefficient;
		float beta = 0.0f;			//quadratic attenuation coefficient;

	};

	int64_t UUID;
	std::string name;
	
	spark::SparkRenderFlags flags;

	spark::Texture diffuse;
	spark::Texture specular;
	spark::Texture normal;

	spark::Instance instance;

	Properties properties;

};

}