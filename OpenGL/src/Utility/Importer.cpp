
#include "Importer.h"

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>

Model importObj(std::string const& filename)
{
	if (!loader.LoadFile(filename))
	{
		std::cerr << ".obj import failed!\nfilepath was: " << filename << std::endl;
		assert(false);
	}

	auto const& verts_struct = loader.LoadedVertices;

	std::cout << "Size of verts is: " << verts_struct.size();

	std::vector<float> verts_raw(verts_struct.size() * 5);

	memcpy(verts_raw.data(), verts_struct.data(), verts_struct.size() * sizeof(float) * 5);
	std::vector<unsigned int> indices_raw = loader.LoadedIndices;

	return Model(std::move(verts_raw), std::move(indices_raw));

}
