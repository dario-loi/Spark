#pragma once
#include "ResourceManager.h"

namespace spark
{
	class Renderer
	{

	public:

		explicit Renderer(spark::ResourceManager& resourceManager)
			: man(resourceManager) {}

		void render();

	private:

		ResourceManager& man;
	};
}