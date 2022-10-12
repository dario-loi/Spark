#pragma once
#include <string>
#include "GL/glew.h"


namespace spark
{

class Texture
{

public:

	Texture(const std::string& filename, const GLenum texType, const GLenum texEnc);
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
	const GLenum texType;
	const GLenum texEnc;

	void initTexture() const;
};

}