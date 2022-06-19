#pragma once
#include <string>
#include "GL/glew.h"

class Texture
{

public:

	Texture(const std::string& filename, const GLenum texType);
	~Texture();
	void Bind(unsigned int channel) const;
	void Unbind() const;
	

private:
	unsigned int RenderID;
	unsigned int currChannel;
	unsigned char* imageBuffer;
	int imgWidth;
	int imgHeight;
	int BitDepth;
	GLenum texType;

	void initTexture() const;
};

