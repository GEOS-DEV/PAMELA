#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#include "MeshDataWriters/EnsightGoldWriter.hpp"
#include "MeshDataWriters/VTKWriter.hpp"
#include "Utils/Logger.hpp"

#include <string>

namespace PAMELA
{
	MeshDataWriter* MeshDataWriterFactory::makeWriter(
                Mesh * mesh,
                const std::string& file_path)
	{
		LOGINFO("**********************************************************************");
		LOGINFO("                         PAMELA Library Export tool                   ");
		LOGINFO("**********************************************************************");


                std::string filename = file_path.substr(file_path.find_last_of("/") + 1);
		std::string file_extension = filename.substr(file_path.find_last_of(".") + 1);
                std::string file_wo_extension
                    = filename.substr(0,file_path.find_last_of("."));
		if ((file_extension == "vtm") || (file_extension == "VTM"))
		{
			LOGINFO("VTM OUTPUT MESH FORMAT IDENTIFIED");
                        VTKWriter * writer = new VTKWriter( mesh,file_wo_extension);
			return  writer;
		}
		if ((file_extension == "msh") || (file_extension == "MSH"))
		{
			LOGINFO("ENSIGHT GOLD OUTPUT MESH FORMAT IDENTIFIED");
                        EnsightGoldWriter * writer =
                            new EnsightGoldWriter( mesh,file_wo_extension);
			return  writer;
		}
		else
		{
			LOGERROR("Mesh file format ." + file_extension + " not supported");
			return nullptr;
		}
	}
}
