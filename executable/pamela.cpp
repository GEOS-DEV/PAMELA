#include <iostream>

#include "command_parser.hpp"
#include "Parallel/Communicator.hpp"
#include "Mesh/MeshFactory.hpp"
#include "MeshDataWriters/MeshDataWriterFactory.hpp" 

#ifdef WITH_VTK
#include<vtkSmartPointer.h>
#include<vtkMPIController.h>
#endif

void add_options(cxxopts::Options& option) {
    option.add_options()
        ("i,input", "Input mesh", cxxopts::value<std::string>())
        ("o,output", "Output mesh", cxxopts::value<std::string>())
        ("h,help", "Print help");
}
int main(int argc, char **argv)
{
    using namespace PAMELA;
    try
    {
        cxxopts::Options options("PAMELA", "PArallel MEsh LibrAry");
        add_options(options);
        auto result = options.parse(argc, argv);

        if (result.count("help") && !result.count("input") && !result.count("output"))
        {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(0);
        }

        if (!result.count("input")) {
            std::cerr<< "No input mesh defined" << std::endl;
            exit(1);
        }

        if (!result.count("output")) {
            std::cerr<< "No output mesh defined" << std::endl;
            exit(1);
        }

        const std::string input_mesh_filename = result["i"].as<std::string>();
        const std::string output_mesh_filename = result["o"].as<std::string>();

	Communicator::initialize();
#ifdef WITH_VTK
        vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
        controler->Initialize(&argc,&argv,true);
        vtkMultiProcessController::SetGlobalController(controler.Get());
#endif
        Mesh* input_mesh = MeshFactory::makeMesh(input_mesh_filename);
	input_mesh->CreateFacesFromCells();
	input_mesh->PerformPolyhedronPartitioning(
                ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MeshDataWriter* output_mesh = MeshDataWriterFactory::makeWriter(
                input_mesh,output_mesh_filename);

	output_mesh->DeclareVariable(
                FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR,
                VARIABLE_LOCATION::PER_CELL, "Partition");
	output_mesh->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR,
                VARIABLE_LOCATION::PER_CELL, "Partition");

	output_mesh->Init();

	output_mesh->SetVariable("Partition", Communicator::worldRank());

	//Dump variables
	output_mesh->DumpVariables();

	Communicator::finalize();


    } catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}
