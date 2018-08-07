#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "Adjacency/Adjacency.hpp"
#include "MeshDataWriters/EnsightGoldWriter.hpp"
#include "MeshDataWriters/VTKWriter.hpp"

int main(int argc, char * argv[]) {

	using namespace  PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();
#ifdef WITH_VTK
        vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
        controler->Initialize(&argc,&argv,true);
        vtkMultiProcessController::SetGlobalController(controler.Get());
#endif

	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/trivial.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/small.mesh");
//	mesh* mainmesh = meshfactory::makemesh("e:/gitlabrepository/parallel mesh library/examples/inria unstructured mesh/final_very_coarse.mesh");
	Mesh * MainMesh = MeshFactory::makeMesh("/home/amazuyer/dev/PAMELA/examples/INRIA Unstructured Mesh/small.mesh");
	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MeshDataWriter* OutputWriter = new VTKWriter(MainMesh, "UnstructuredGridExample");
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	//Writer->AddElementScalarVariable("Pressure");
	
	OutputWriter->Init();

	OutputWriter->SetVariable("Partition", Communicator::worldRank());

	//Dump variables
	OutputWriter->Dump();

	Communicator::finalize();

	return 0;
}

