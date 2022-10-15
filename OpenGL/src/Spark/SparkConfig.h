#pragma once
#include <cstdint>
#include <cstddef>

#define TO_INT8(X) static_cast<uint8_t>(X)

namespace spark
{
	//constants
	inline constexpr int16_t NUM_LIGHTS = 1024;

	//enums
	enum class SparkRenderFlags : int8_t
	{
		/**
		 * Specifies how an object should be treated by the renderer/shader.
		 */
		NONE				= 0x0,			//To be used for bitwise ops.
		HAS_DIFFUSE_MAP		= 0x1 << 0,		//SparkObject has an associated diffuse map.
		HAS_SPECULAR_MAP	= 0x1 << 1,		//SparkObject has an associated specular map.
		HAS_NORMAL_MAP		= 0x1 << 2,		//SparkObject has an associated normal map.
		HAS_EMISSIVE_MAP	= 0x1 << 3		//SparkObject has an emissive map (and therefore must be a light source).
	};

	enum class SparkVAOLayouts : int8_t
	{
		/**
		 * Specifies different elements that
		 * can be present in a VAO layout,
		 *
		 * Layouts components MUST be organized sequentially
		 * in the same order as below in order to be compatible.
		 */

		NONE = 0x0,
		HAS_POSITION = 0x1 << 0,
		HAS_NORMAL = 0x1 << 1,
		HAS_UVS = 0x1 << 2,
		HAS_BITANGENT = 0x1 << 3,
		HAS_COLOR = 0x1 << 4

	};

	enum class SparkTextureType : int8_t
	{
		/**
		 * Indicates a texture's role.
		 */
		DIFFUSE_MAP		= 0x0,
		SPECULAR_MAP	= 0x1,
		NORMAL_MAP		= 0x2,
		EMISSIVE_MAP	= 0x3
	};

	/*
	
	Defining bitwise OR operators for our enums
	
	*/

	constexpr SparkRenderFlags operator|(SparkRenderFlags const lhs, SparkRenderFlags const rhs) {
		return static_cast<SparkRenderFlags>(TO_INT8(lhs) | TO_INT8(rhs));
	}

	constexpr SparkRenderFlags& operator|=(SparkRenderFlags& lhs, SparkRenderFlags const& rhs) {
		return lhs = lhs | rhs;
	}

	constexpr SparkTextureType operator|(SparkTextureType const lhs, SparkTextureType const rhs) {
		return static_cast<SparkTextureType>(TO_INT8(lhs) | TO_INT8(rhs));
	}

	constexpr SparkTextureType& operator|=(SparkTextureType& lhs, SparkTextureType const& rhs) {
		return lhs = lhs | rhs;
	}

	constexpr SparkVAOLayouts operator|(SparkVAOLayouts const lhs, SparkVAOLayouts const rhs) {
		return static_cast<SparkVAOLayouts>(TO_INT8(lhs) | TO_INT8(rhs));
	}

	constexpr SparkVAOLayouts& operator|=(SparkVAOLayouts& lhs, SparkVAOLayouts const& rhs) {
		return lhs = lhs | rhs;
	}

	/*

	Defining bitwise AND operators for our enums

	*/

	constexpr SparkRenderFlags operator&(SparkRenderFlags const lhs, SparkRenderFlags const rhs) {
		return static_cast<SparkRenderFlags>(TO_INT8(lhs) & TO_INT8(rhs));
	}

	constexpr SparkRenderFlags& operator&=(SparkRenderFlags& lhs, SparkRenderFlags const& rhs) {
		return lhs = lhs & rhs;
	}

	constexpr SparkTextureType operator&(SparkTextureType const lhs, SparkTextureType const rhs) {
		return static_cast<SparkTextureType>(TO_INT8(lhs) & TO_INT8(rhs));
	}

	constexpr SparkTextureType& operator&=(SparkTextureType& lhs, SparkTextureType const& rhs) {
		return lhs = lhs & rhs;
	}

	constexpr SparkVAOLayouts operator&(SparkVAOLayouts const lhs, SparkVAOLayouts const rhs) {
		return static_cast<SparkVAOLayouts>(TO_INT8(lhs) & TO_INT8(rhs));
	}

	constexpr SparkVAOLayouts& operator&=(SparkVAOLayouts& lhs, SparkVAOLayouts const& rhs) {
		return lhs = lhs & rhs;
	}

	/*

	Defining bitwise NOT operators for our enums

	*/

	constexpr SparkRenderFlags operator~(SparkRenderFlags val) {
		return static_cast<SparkRenderFlags>(~TO_INT8(val));
	}

	constexpr SparkTextureType operator~(SparkTextureType val) {
		return static_cast<SparkTextureType>(~TO_INT8(val));
	}

	constexpr SparkTextureType operator~(SparkVAOLayouts val) {
		return static_cast<SparkVAOLayouts>(~TO_INT8(val));
	}
}




