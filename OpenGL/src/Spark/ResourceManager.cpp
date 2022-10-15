#include "ResourceManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "SparkConfig.h"
#include "..\Utility\Importer.h"

namespace spark 
{

	ResourceManager::ResourceManager()
		: UUIDgen{spark::spark_UUID}
	{
		ImGui::Begin("Resource Manager");
		ImGui::Text("This will have all sorts of awesome functionalities!");
		ImGui::End();
	}

	void ResourceManager::spawnObject(std::shared_ptr<spark::Model>&& model, glm::vec4 const& position, std::string const& name)
	{
		

		models.emplace(std::move(model));
		objMap.emplace(UUIDgen(name), name, model);

	}

	void ResourceManager::spawnObject(std::string const& model_name, [[maybe_unused]] glm::vec4 const& position = glm::vec4(0.0F, 0.0F, 0.0F, 1.0F), std::string const& name)
	{
		objMap.emplace(
			UUIDgen(name), name, *(models.get<spark::ModelName>().find(name))
			);

		/*TODO: Set object position (waiting for physics :) */
	}

	void ResourceManager::spawnObject(boost::uuids::uuid model_UUID, glm::vec4 const& position, std::string const& name)
	{
		objMap.emplace(
			UUIDgen(name), name, *(models.get<spark::UniqueID>().find(model_UUID))
		);
	}

	void ResourceManager::addModel(std::shared_ptr<spark::Model>&& model)
	{
		models.emplace(std::move(model));
	}

	void ResourceManager::importModel(std::string const& model_path, std::string const& name, SparkVAOLayouts layout_specifier)
	{

		auto ret = sparkutils::importObj(model_path);
		Model mod{ UUIDgen(name), name, std::move(ret.verts), std::move(ret.indx) };
		
		if(layout_specifier & SparkVAOLayouts::HAS_POSITION)

		models.emplace();
	}

	void ResourceManager::loadShader(std::string const& shader_path, std::string const& name)
	{

		shaders.emplace(UUIDgen(name), shader_path, name);

	}

	void ResourceManager::addTexture(std::string const& texture_path, std::string const& name, SparkTextureType texType)
	{
		GLenum internalEncoding;
		GLenum internalType;

		switch (texType)
		{
		case SparkTextureType::DIFFUSE_MAP:
			internalEncoding = GL_TEXTURE_2D;
			internalType = GL_SRGB8_ALPHA8;
			break;
		case SparkTextureType::SPECULAR_MAP:
			internalEncoding = GL_TEXTURE_2D;
			internalType = GL_RGBA8;
			break;
		case SparkTextureType::NORMAL_MAP:
			internalEncoding = GL_TEXTURE_2D;
			internalType = GL_RGBA8;
			break;
		case SparkTextureType::EMISSIVE_MAP:
			internalEncoding = GL_TEXTURE_2D;
			internalType = GL_RGBA8;
			break;
		default:
#ifdef _DEBUG
			assert("SparkTextureType provided to Resource Manager does not exist");
#endif // _DEBUG
		}

		textures.emplace(UUIDgen(name), name, texture_path,
			internalType, internalEncoding, texType);
	}

}

