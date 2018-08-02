#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/EnsightGoldWriter.hpp"

int main(int argc, const char * argv[]) 
{

	using namespace PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(5));

	Communicator::initialize();

	//Make mesh
	Mesh* MainMesh = MeshFactory::makeMesh(2, 2, 1, 1./5, 1./3, 1./4);
	MainMesh->CreateFacesFromCells();

	//Distort Mesh
	MainMesh->Distort(0.0);
	
	//Partitioning
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	//Output
	//--Create 
	MeshDataWriter* OutputWriter = new EnsightGoldWriter(MainMesh, "Cartesian");
	//--Create variables
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL,"Pressure");
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_NODE, "Test");

	//--Make files
	OutputWriter->Init();

	//--Set variables
	//Set variable values
	OutputWriter->SetVariable("Pressure", 55);
	OutputWriter->SetVariable("Test", 0.1);
	OutputWriter->SetVariable("Partition", Communicator::worldRank());
	
	//Dump variables
	OutputWriter->Dump();

	Communicator::finalize();

	return 0;
}


