#pragma once
#include <string>
#include "tiny_obj_loader.h"
#include "../GLData/Model.h"

namespace sparkutils
{

	struct SparkImportReturn
	{
		std::vector<float> verts;
		std::vector<unsigned int> indx;
	};


	SparkImportReturn importObj(std::string const& filename);

}
