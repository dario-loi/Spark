#pragma once

#include "SparkMultiIdxDefs.h"

#include <vector>
#include <map>
#include <cstdint>
#include <memory>

#include "vec4.hpp"

#include "../GLData/Buffers/UBO.h"
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

		[[nodiscard]] spark::SparkObjContainer getObjects() const
		{
			return objMap;
		}

		spark::SparkObjContainer& getObjects()
		{
			return objMap;
		}


		[[nodiscard]] spark::SparkTextureContainer getTextures() const
		{
			return textures;
		}

		spark::SparkTextureContainer& getTextures()
		{
			return textures;
		}

		[[nodiscard]] spark::SparkShaderContainer getShaders() const
		{
			return shaders;
		}

		spark::SparkShaderContainer& getShaders() 
		{
			return shaders;
		}

		[[nodiscard]] spark::SparkModelContainer getModels() const
		{
			return models;
		}

		spark::SparkModelContainer& getModels()
		{
			return models;
		}

		[[nodiscard]] std::shared_ptr<spark::Shader> getShader(std::string const& name) const noexcept
		{
			return *shaders.get<ShaderName>().find(name);
		}

		auto& getLights() { return lights; }
		[[nodiscard]] auto getLights() const { return lights; }

		auto& getLightUBO() { return light_positions; }

		auto& getTransparents() { return transparents; }
		void sortTransparents();

		[[nodiscard]] auto getTransparents() const { return transparents; }

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
		void importModel(std::string const& model_path, std::string const& name, SparkVAOLayouts layout_specifier);

		/**
		 * Loads a glsl shader to be used by Spark.
		 */
		void loadShader(std::string const& shader_path, std::string const& name);
		void setActiveShader(std::string const& name) const noexcept;


		void addTexture(std::string const& texture_path, std::string const& name, SparkTextureType texType);

	private:


		std::vector<std::unique_ptr<spark::SparkObject>> lights;
		std::vector<std::unique_ptr<spark::SparkObject>> transparents;

		UBO<glm::vec4> light_positions{SPARK_UBO_LIGHT_CHANNEL};

		spark::SparkTextureContainer textures;
		spark::SparkModelContainer models;
		spark::SparkShaderContainer shaders;
		spark::SparkObjContainer objMap;

		boost::uuids::name_generator_sha1 UUIDgen;
	};

}
