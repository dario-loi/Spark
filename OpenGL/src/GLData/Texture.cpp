#include "Texture.h"

#include <iostream>
#include <assert.h>

#include "..\..\res\vendor\stb_image.h"

spark::Texture::Texture(boost::uuids::uuid UUID_, std::string const& name_,
	std::string const& filename, const GLenum texType, const GLenum texEnc, spark::SparkTextureType type_)
	: UUID(UUID_), name(name_), RenderID(0), currChannel(0), 
	imgWidth(0), imgHeight(0), BitDepth(4), texType(texType), texEnc(texEnc), sparkType(type_)
{

	stbi_set_flip_vertically_on_load(1);
	imageBuffer = stbi_load(filename.c_str(), &imgWidth, &imgHeight, &BitDepth, BitDepth);

	glGenTextures(1, &RenderID);
	
	if (!imageBuffer)
	{
		std::cerr << "Failed to load texture at address: " + filename + ", Aborting!" << std::endl;
		assert(false);
	}

	initTexture();
	Unbind();
}

/**
 * Frees the stbi buffer.
 * 
 */
spark::Texture::~Texture()
{
	Unbind();
	stbi_image_free(imageBuffer);
}

/**
*  \brief Allows and incapsulates different initializations based on the texture's type.
*
*  \return void
*/
void spark::Texture::initTexture() const
{
	switch (texType)
	{
	case GL_TEXTURE_2D:

		glBindTexture(texType, RenderID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, texEnc, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		glGenerateMipmap(texType);
		break;

	case GL_TEXTURE_CUBE_MAP:

		glBindTexture(texType, RenderID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (unsigned int face = 0; face < 6; face++)
		{
			std::cerr << "Currently processing face number: " << face << std::endl;
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, texEnc, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer
			);
		}

		glGenerateMipmap(texType);
		break;

	default:
		assert(true, 'Unsupported Texture type, come back later!');
		break;
	}
}

void spark::Texture::Bind(unsigned int channel) const
{
	if (int TexSlot = GL_TEXTURE0 + channel; TexSlot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	{
		glActiveTexture(TexSlot);
	}
	else
	{
		std::cerr << "Currently selected texture slot (" << channel << ") is unavailable in " <<
			"the current platform, overwriting slot (0)" << std::endl;
		glActiveTexture(GL_TEXTURE0);
	}
	glBindTexture(texType, RenderID);
}

void spark::Texture::Unbind() const
{
	glBindTexture(texType, 0);
	glDisable(texType);
}