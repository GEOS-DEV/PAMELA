#include <iostream>
#include <fstream>
#include <string>

#include "Mesh/MeshFactory.hpp"
#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#include "tests_config.h"

namespace PAMELA {
    void save(const std::string& filename_out, Mesh * input_mesh) {
        MeshDataWriter* output_mesh = MeshDataWriterFactory::makeWriter(
                input_mesh,filename_out);

        output_mesh->DeclareVariable(
                FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR,
                VARIABLE_LOCATION::PER_CELL, "Partition");

        output_mesh->SetVariable("Partition", Communicator::worldRank());

        output_mesh->Init();


        //Dump variables
        output_mesh->DumpVariables();
    }
    void load_and_save( std::ifstream& input) {
        std::string line;
        while (std::getline(input, line)) {
            const std::string filename_in = test_data_path + line;
            Mesh* input_mesh = MeshFactory::makeMesh(filename_in);
            input_mesh->CreateFacesFromCells();
            input_mesh->PerformPolyhedronPartitioning(
                    ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

            std::string filename = filename_in.substr(filename_in.find_last_of("/") + 1);
            std::string file_wo_extension
                = filename.substr(0,filename.find_last_of("."));
            const std::string filename_out = test_output_path + file_wo_extension;
            save(filename_out+".vtm",input_mesh);
            save(filename_out+".case",input_mesh);
        }
    }
}
