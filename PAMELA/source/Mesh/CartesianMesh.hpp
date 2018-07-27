#pragma once
#include "Mesh/Mesh.hpp"

namespace PAMELA
{

	class CartesianMesh : public Mesh
	{

	public:

		CartesianMesh(const std::vector<double>& dx, const std::vector<double>& dy, const std::vector<double>& dz);
		void Distort(double alpha);


	private:

		std::vector<double> m_dx, m_dy, m_dz;
		double m_Lxmin, m_Lymin, m_Lzmin, m_Lxmax, m_Lymax, m_Lzmax;

	};

}