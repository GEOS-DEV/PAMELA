#pragma once
#include <vector> 
#include "MeshDataWriters/MeshDataWriter.hpp"

namespace PAMELA
{
	class MeshDataWriterFactory
	{

	public:

		static MeshDataWriter* makeWriter(Mesh * mesh,
                        const std::string& file_path);

	private:
		MeshDataWriterFactory() = delete;

	};

}
