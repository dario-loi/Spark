#pragma once
#include <string>
#include "OBJ_Loader.h"
#include "../GLData/Model.h"


Model importObj(std::string const& filename);

extern inline objl::Loader loader{};
