#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/mesh.hpp"
#include "Utils/Communicator.hpp"
#include <thread>
#include "Adjacency/Adjacency.hpp"
#include "MeshDataWriters/EnsightGoldWriter.hpp"

int main(int argc, const char * argv[]) {


	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();

	//Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/trivial.mesh");
	Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/small.mesh");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/LocalGitRepository/PAMELA - PArallel MEsh LibrAry/examples/INRIA Unstructured Mesh/spe10_one_layer.mesh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	EnsightGold::EnsightGoldWriter* Writer = new EnsightGold::EnsightGoldWriter(MainMesh, "UnstructuredGridExample");
	//Writer->AddElementScalarVariable("Pressure");
	Writer->MakeCaseFile();
	Writer->MakeGeoFile();

	Communicator::finalize();

	return 0;
}
