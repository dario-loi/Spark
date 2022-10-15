#include "SparkObject.h"

void spark::SparkObject::setDiffuse(std::shared_ptr<spark::Texture> const& diff)
{
	textures.diffuse = diff;
	flags |= spark::SparkRenderFlags::HAS_DIFFUSE_MAP;

}

void spark::SparkObject::setSpecular(std::shared_ptr<spark::Texture> const& spec)
{
	textures.diffuse = spec;
	flags |= spark::SparkRenderFlags::HAS_SPECULAR_MAP;
}

void spark::SparkObject::setNormal(std::shared_ptr<spark::Texture> const& norm)
{
	textures.normal = norm;
	flags |= spark::SparkRenderFlags::HAS_NORMAL_MAP;
}

void spark::SparkObject::setEmissive(std::shared_ptr<spark::Texture> const& emi)
{
	textures.emissive = emi;
	flags |= spark::SparkRenderFlags::HAS_EMISSIVE_MAP;
}
