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

	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/trivial.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/small.mesh");
	Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/final_very_coarse.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/spe10_one_layer.mesh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MeshDataWriter* Writer = new EnsightGoldWriter(MainMesh, "UnstructuredGridExample");
	Writer->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	//Writer->AddElementScalarVariable("Pressure");
	
	Writer->Init();

	Writer->SetVariable("Partition", Communicator::worldRank());

	//Dump variables
	Writer->DumpVariables();

	Communicator::finalize();

	return 0;
}
