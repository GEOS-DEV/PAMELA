#pragma once
#include "Mesh/MeshFactory.hpp"
#include "Import/INRIA_mesh.hpp"
#include "Utils/Logger.hpp"
#include "Mesh/CartesianMesh.hpp"
#include "Import/Gmsh_mesh.hpp"

namespace PAMELA
{
	/**
	 * \brief
	 * \param file_path
	 * \return
	 */
	Mesh* MeshFactory::makeMesh(std::string file_path)
	{
		std::string file_extension;
		file_extension = file_path.substr(file_path.find_last_of(".") + 1);

		if ((file_extension == "mesh") || (file_extension == "MESH"))
		{
			return INRIA_mesh::CreateMesh(file_path);
		}
		if ((file_extension == "msh") || (file_extension == "MSH"))
		{
			return Gmsh_mesh::CreateMesh(file_path);
		}
		else
		{
			LOGERROR("Mesh file format ." + file_extension + " not supported");
			return nullptr;
		}
	}

	/**
	 * \brief
	 * \param nx
	 * \param ny
	 * \param nz
	 * \param dx
	 * \param dy
	 * \param dz
	 * \return
	 */
	Mesh* MeshFactory::makeMesh(int nx, int ny, int nz, double dx, double dy, double dz)
	{

		std::vector<double> vdx, vdy, vdz;
		for (auto i = 0; i < nx; i++)
		{
			vdx.push_back(dx);
		}
		for (auto i = 0; i < ny; i++)
		{
			vdy.push_back(dy);
		}
		for (auto i = 0; i < nz; i++)
		{
			vdz.push_back(dz);
		}

		return new CartesianMesh(vdx, vdy, vdz);

	}
}
