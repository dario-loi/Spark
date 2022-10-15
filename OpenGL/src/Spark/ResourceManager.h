#pragma once



#include "SparkMultiIdxDefs.h"

#include <vector>
#include <map>
#include <cstdint>
#include <memory>

#include "vec4.hpp"

#include "SparkConfig.h"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/name_generator.hpp"
#include "SparkObject.h"
#include "../GLData/Shader.h"

namespace spark
{
	class ResourceManager
	{
	public:

		ResourceManager();

		void spawnObject(std::shared_ptr<spark::Model>&& model, glm::vec4 const& position, std::string const& name);
		void spawnObject(std::string const& model_name, glm::vec4 const& position, std::string const& name);
		void spawnObject(boost::uuids::uuid model_UUID, glm::vec4 const& position, std::string const& name);

		/**
		 * Gives the option to "Register" an orphaned model,
		 * his should generally *not* be allowed and might be deprecated.
		 */

		void addModel(std::shared_ptr<spark::Model>&& model);

		/**
		 * Imports a Wavefront .Obj file into a model and registers
		 * it into the manager's collections.
		 */
		void importModel(std::string const& model_path, std::string const& name);

		/**
		 * Loads a glsl shader to be used by Spark.
		 */
		void loadShader(std::string const& shader_path, std::string const& name);


		void addTexture(std::string const& model_path, std::string const& name, SparkTextureType texType);

	private:

		std::vector<std::unique_ptr<spark::SparkObject>> lights;
		std::vector<std::unique_ptr<spark::SparkObject>> transparents;

		spark::SparkTextureContainer textures;
		spark::SparkModelContainer models;
		spark::SparkShaderContainer shaders;
		spark::SparkObjContainer objMap;

		boost::uuids::name_generator_sha1 UUIDgen;
	};

}
