#pragma once
// Library includes
#include "MeshDataWriters/MeshDataWriter.hpp"

#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	namespace VTK_
	{

		class VTKWriter;

	}

}