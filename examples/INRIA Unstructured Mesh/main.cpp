#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "Adjacency/Adjacency.hpp"
#include "MeshDataWriters/EnsightGoldWriter.hpp"
#include "MeshDataWriters/VTKWriter.hpp"
#include "MeshDataWriters/MeshDataWriterFactory.hpp"

int main(int argc, char * argv[]) {

	using namespace  PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();

	Mesh* MainMesh = MeshFactory::makeMesh("../../../data/medit/small.mesh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "UnstructuredGrid.case");
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

