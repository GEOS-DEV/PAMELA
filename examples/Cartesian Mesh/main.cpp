#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Utils/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/EnsightGoldWriter.hpp"

int main(int argc, const char * argv[]) 
{

	//std::this_thread::sleep_for(std::chrono::seconds(5));

	Communicator::initialize();

	//Make mesh
	Mesh* MainMesh = MeshFactory::makeMesh(5, 5, 5, 1, 1, 1);
	MainMesh->CreateFacesFromCells();

	//Distort Mesh
	MainMesh->Distort(0.0);
	
	//Partitioning
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	//Output
	//--Create 
	EnsightGold::EnsightGoldWriter* OutputWriter = new EnsightGold::EnsightGoldWriter(MainMesh, "Cartesian");
	//--Create variables
	OutputWriter->CreateVariable(EnsightGold::FAMILY::POLYHEDRON, EnsightGold::ENSIGHT_GOLD_VARIABLE_TYPE::SCALAR, EnsightGold::ENSIGHT_GOLD_VARIABLE_LOCATION::PER_CELL, "Partition", "DEFAULT");
	OutputWriter->CreateVariable(EnsightGold::FAMILY::POLYHEDRON, EnsightGold::ENSIGHT_GOLD_VARIABLE_TYPE::SCALAR,EnsightGold::ENSIGHT_GOLD_VARIABLE_LOCATION::PER_CELL,"Pressure","DEFAULT");

	//--Make files
	OutputWriter->MakeCaseFile();
	OutputWriter->MakeGeoFile();

	//--Set variables
	//Set variable values
	OutputWriter->SetVariable("Pressure", 55);
	OutputWriter->SetVariable("Partition", Communicator::worldRank());
	
	//Dump variables
	OutputWriter->DumpVariables();

	Communicator::finalize();

	return 0;
}


