#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Utils/Communicator.hpp"
#include <thread>
#include "Adjacency/Adjacency.hpp"
#include "MeshDataWriters/EnsightGoldWriter.hpp"

int main(int argc, const char * argv[]) {

	using namespace  PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();

	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/tiny.mesh");
	Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/small.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/final_very_coarse.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/spe10_one_layer.mesh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MeshDataWriter* OutputWriter = new EnsightGoldWriter(MainMesh, "UnstructuredGridExample");
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	//Writer->AddElementScalarVariable("Pressure");
	
	OutputWriter->Init();

	OutputWriter->SetVariable("Partition", Communicator::worldRank());

	//Dump variables
	OutputWriter->DumpVariables();

	Communicator::finalize();

	return 0;
}
