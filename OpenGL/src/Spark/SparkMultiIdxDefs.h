#pragma once
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "SparkObject.h"
#include "..\GLData\Shader.h"
#include "..\GLData\Texture.h"

namespace spark
{

	/*
	Namespace ID (for random number generator initialization)

	Lack of documentation leads to the assumption that this 
	is default-initialized to a non-trivial value.
	*/
	boost::uuids::uuid const spark_UUID; 

	/*
		SPARK_OBJECT
	*/

	//boost::multi_index tag
	struct UniqueID {};
	//boost::multi_index tag
	struct ObjectName {};

	using SparkObjContainer = boost::multi_index::multi_index_container< std::shared_ptr<spark::SparkObject>,
		boost::multi_index::indexed_by<
		boost::multi_index::hashed_unique<
		boost::multi_index::tag<UniqueID>,
		boost::multi_index::const_mem_fun<spark::SparkObject, const boost::uuids::uuid, &spark::SparkObject::getUUID>
		>,
		boost::multi_index::hashed_non_unique<
		boost::multi_index::tag<ObjectName>,
		boost::multi_index::const_mem_fun<spark::SparkObject, std::string, &spark::SparkObject::getModelName>
		>
		>
	>;

	/*
		SPARK_MODEL
	*/

	//boost::multi_index tag
	struct ModelName {};

	using SparkModelContainer = boost::multi_index::multi_index_container< std::shared_ptr<spark::Model>,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique<
			boost::multi_index::tag<UniqueID>,
			boost::multi_index::const_mem_fun<spark::Model, const boost::uuids::uuid, &spark::Model::getUUID>
		>,
			boost::multi_index::hashed_unique<
			boost::multi_index::tag<ModelName>,
			boost::multi_index::const_mem_fun<spark::Model, std::string, &spark::Model::getName>
			>
		>
	>;

	/*
	SPARK_SHADER
	*/

	//boost::multi_index tag
		struct ShaderName {};

		using SparkShaderContainer = boost::multi_index::multi_index_container< std::shared_ptr<spark::Shader>,
			boost::multi_index::indexed_by<
			boost::multi_index::hashed_unique<
			boost::multi_index::tag<UniqueID>,
			boost::multi_index::const_mem_fun<spark::Shader, const boost::uuids::uuid, &spark::Shader::getUUID>
			>,
			boost::multi_index::hashed_unique<
			boost::multi_index::tag<ShaderName>,
			boost::multi_index::const_mem_fun<spark::Shader, std::string, &spark::Shader::getName>
			>
			>
		>;

	/*
	SPARK_TEXTURE
	*/

	//boost::multi_index tag
		struct TextureName {};

		using SparkTextureContainer = boost::multi_index::multi_index_container < std::shared_ptr<spark::Texture>,
			boost::multi_index::indexed_by <
			boost::multi_index::hashed_unique <
			boost::multi_index::tag<UniqueID>,
			boost::multi_index::const_mem_fun<spark::Texture, const boost::uuids::uuid, &spark::Texture::getUUID>
			>,
			boost::multi_index::hashed_unique<
			boost::multi_index::tag<TextureName>,
			boost::multi_index::const_mem_fun<spark::Texture, std::string, &spark::Texture::getName>
			>
			>
		>;
}


