#pragma once
#include <string>

class Texture
{
private:
	unsigned int RenderID;
	unsigned int currChannel;
	unsigned char* imageBuffer;
	int imgWidth;
	int imgHeight;
	int BitDepth;

public:

	Texture(const std::string& filename);
	void Bind(unsigned int channel) const;
	void Unbind() const;


};

