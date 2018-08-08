#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Adjacency/Adjacency.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/EnsightGoldWriter.hpp"
#include "MeshDataWriters/MeshDataWriterFactory.hpp"

int main(int argc, const char * argv[]) {

	using namespace  PAMELA;


	Communicator::initialize();

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Mesh* MainMesh = MeshFactory::makeMesh("../../../data/eclipse/ReducedNorne/IRAP_1005.GRDECL");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	////-------------------------Output
	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "EclipseGrid.case");

	//Variable declarations
	
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");

	auto mesh_props = MainMesh->get_PolyhedronProperty()->get_PropertyMap();
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, it->first);
	}

	//
	OutputWriter->DeclareAdjacency("Volume to Volume",MainMesh->getMeshAdjacency()->get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON));

	//Init
	OutputWriter->Init();

	//Set
	OutputWriter->SetVariable("Partition", Communicator::worldRank());
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->SetVariable(it->first, it->second);
	}

	//Dump
	OutputWriter->Dump();

	Communicator::finalize();

        
	return 0;
}
