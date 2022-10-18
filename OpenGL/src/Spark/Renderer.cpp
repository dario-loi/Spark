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
		return { (*objsPtr.get()).getInstance().getTransform().vDisplacement.vector, 1.0F };
		});

	
	man.getLightUBO().setData(std::move(positions));

	auto const& models = man.getModels();
	auto& objects = man.getObjects();

	for (auto const& mod : models)
	{
		//Instance Rendering for each model
		Model const& model = *mod;
		model.Bind();

		/*
			Loop objects with the same model and prepare their VBOs by
			updating their instance data
		*/
		auto [obj_it, obj_end] = objects.get<ModelName>().equal_range(model.getName());
		std::vector<spark::SparkInstanceData> buffer(std::distance(obj_end, obj_it));

		for (;obj_it != obj_end;++obj_it)
		{
			auto& object = *(* obj_it);

			buffer.emplace_back(
				object.getInstance().getModelMatrix(), 
				object.getInstance().getNormalMatrix(), 
				object.getMaterial());
			
		}

		model.getVBO().setInstanceData(buffer);

		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)model.getIndexSize(),
			GL_UNSIGNED_INT, nullptr, model.getVBO().getInstSize());

	}
	
	/*
	
		Draw transparents

	*/

}
