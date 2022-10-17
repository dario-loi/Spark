#pragma once
#include "ResourceManager.h"

namespace spark
{
	class Renderer
	{

	public:

		Renderer(spark::ResourceManager const& resourceManager)
			: man(resourceManager) {}

		void render();

	private:

		ResourceManager const& man;
	};
}