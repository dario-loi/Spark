#pragma once
#include <stdint.h>

namespace spark
{
	//constants
	inline constexpr int16_t NUM_LIGHTS = 1024;


	//enums
	enum SparkRenderFlags : int8_t //change to something more *meaningful
	{
		NONE				= 0x0,			//To be used for bitwise ops.
		HAS_SPECULAR_MAP	= 0x1 << 0,		//SparkObject has an associated specular map.
		HAS_NORMAL_MAP		= 0x1 << 1,		//SparkObject has an associated normal map.
		HAS_EMISSIVE_MAP	= 0x1 << 2		//SparkObject has an emissive map (and therefore must be a light source).
	};

	enum SparkTextureType : int8_t
	{
		DIFFUSE_MAP		= 0x0,
		SPECULAR_MAP	= 0x1,
		NORMAL_MAP		= 0x2,
		EMISSIVE_MAP	= 0x3
	};


}

