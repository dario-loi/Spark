#pragma once
#include <string>
#include "tiny_obj_loader.h"
#include "../GLData/Model.h"

namespace sparkutil
{

	spark::Model importObj(std::string const& filename);

}
