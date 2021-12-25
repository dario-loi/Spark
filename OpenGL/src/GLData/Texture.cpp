#include "Texture.h"
#include "GL\glew.h"
#include <iostream>
#include <cassert>

#include "..\..\res\vendor\stb_image.h"

Texture::Texture(const std::string& filename)
	: currChannel(0), RenderID(0), imgWidth(0), imgHeight(0), BitDepth(4)
{
	stbi_set_flip_vertically_on_load(1);
	imageBuffer = stbi_load(filename.c_str(), &imgWidth, &imgHeight, &BitDepth, BitDepth);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &RenderID);

	glBindTexture(GL_TEXTURE_2D, RenderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Bind(unsigned int channel) const
{
	int TexSlot = GL_TEXTURE0 + channel;
	if (TexSlot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	{
		glActiveTexture(TexSlot);
	}
	else
	{
		assert(true);
		std::cerr << "Currently selected texture slot (" << channel << ") is unavailable in " <<
			"the current platform, overwriting slot (0)" << std::endl;
		glActiveTexture(GL_TEXTURE0);
	}
	glBindTexture(GL_TEXTURE_2D, RenderID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
