#pragma once

#include "GL/glew.h"
#include "GLData/IndexBuffer.h"
#include "GLData/VertexArray.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();

class Renderer
{

private:


public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;

};
