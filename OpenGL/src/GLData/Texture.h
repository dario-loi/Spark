#pragma once
#include <string>
#include "GL/glew.h"
#include <cstdint>
#include "..\Spark\SparkConfig.h"
#include "boost\uuid\uuid.hpp"

namespace spark
{
	class Texture
	{

	public:

		Texture(boost::uuids::uuid UUID_, std::string const& name_,
			std::string const& filename, const GLenum texType, const GLenum texEnc, spark::SparkTextureType type_);
		~Texture();
		void Bind(unsigned int channel) const;
		void Unbind() const;

		

		[[nodiscard]] std::string getName() const
		{
			return name;
		}

		[[nodiscard]] const boost::uuids::uuid getUUID() const
		{
			return UUID;
		}

	private:
		const boost::uuids::uuid UUID;
		std::string name;

		unsigned int RenderID;
		unsigned int currChannel;
		unsigned char* imageBuffer;

		int imgWidth;
		int imgHeight;
		int BitDepth;

		const GLenum texType;
		const GLenum texEnc;
		spark::SparkTextureType sparkType;

		void initTexture() const;
	};
}