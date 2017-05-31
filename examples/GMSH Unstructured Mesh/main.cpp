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

	//Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PArallel MEsh LibrAry/examples/GMSH Unstructured Mesh/THM.msh");
	Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PArallel MEsh LibrAry/examples/GMSH Unstructured Mesh/BoxWithFracs.msh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/spe10_one_layer.mesh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);


	EnsightGold::EnsightGoldWriter* OutputWriter = new EnsightGold::EnsightGoldWriter(MainMesh, "UnstructuredGridExample");
	OutputWriter->CreateVariable(EnsightGold::FAMILY::POLYHEDRON, EnsightGold::ENSIGHT_GOLD_VARIABLE_TYPE::SCALAR, EnsightGold::ENSIGHT_GOLD_VARIABLE_LOCATION::PER_CELL, "Partition", "POLYHEDRON_GROUP_9999991");
	OutputWriter->MakeCaseFile();
	OutputWriter->MakeGeoFile();

	OutputWriter->SetVariable("Partition", Communicator::worldRank());

	OutputWriter->DumpVariables();
	Communicator::finalize();

	return 0;
}
