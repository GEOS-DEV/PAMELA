#pragma once
#include <string>
#include <vector> 
#include "Mesh/UnstructuredMesh.hpp"

namespace PAMELA
{

	class Mesh;

	class MeshFactory
	{

	public:

		static Mesh* makeMesh(std::string file_path);
		static Mesh* makeMesh(int nx, int ny, int nz, double dx, double dy, double dz);

	private:
		MeshFactory() = delete;

	};

}
