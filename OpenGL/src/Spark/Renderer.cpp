#include "Renderer.h"
#include <algorithm>
void spark::Renderer::render()
{
	using ptrToObjs = std::shared_ptr<spark::SparkObject>;

	/*
		Update Uniform Buffer Object to contain
		the positions of the lights.
	*/
	auto const& lights = man.getLights();
	std::vector<glm::vec4> positions(lights.size());

	std::transform(lights.begin(), lights.end(), positions.begin(), [](ptrToObjs const& objsPtr) -> glm::vec4 {
		return { (*objsPtr.get()).getInstance().getTransform().vDisplacement.vector, 1.0f };
		});

	
	man.getLightUBO().setData(std::move(positions));

	auto const& models = man.getModels();
	auto& objects = man.getObjects();

	for (auto const mod : models)
	{
		//Instance Rendering for each model
		Model& model = *mod.get();

		/*
			Loop objects with the same model and prepare their VBOs by
			updating their instance data
		*/
		for (auto [obj_it, obj_end] = objects.get<ModelName>().equal_range(model.getName());
			obj_it != obj_end;
			++obj_it)
		{
			auto& object = *obj_it->get();
			auto mm = object.getInstance().getModelMatrix();
			auto nm = object.getInstance().getNormalMatrix();
			auto props = object.getMaterial();

			/*
			* stuff this into VBO
			*/
			
		}

		/*
			Send an GlDrawArraysInstanced call
		*/

	}

	/* 
		Draw objects with transparencies
	*/

}
