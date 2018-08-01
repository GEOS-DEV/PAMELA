#include "PAMELA.hpp"
#include <iostream>
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/EnsightGoldWriter.hpp"

int main(int argc, const char * argv[]) {

	using namespace  PAMELA;

	std::this_thread::sleep_for(std::chrono::seconds(2));
	Communicator::initialize();

	//Mesh* MainMesh = MeshFactory::makeMesh("E:\GitLabRepository\PArallel MEsh LibrAry\examples/ECLIPSE Mesh/SAIGUP/SAIGUP.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/ECLIPSE Mesh/Johansen/FULLFIELD_IMAXJMAX.GRDECL");
	Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/ECLIPSE Mesh/Norne/GRID/IRAP_1005.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:\GitLabRepository\PArallel MEsh LibrAry\examples/ECLIPSE Mesh/ReducedNorne/IRAP_1005.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:\GitLabRepository\PArallel MEsh LibrAry\examples/ECLIPSE Mesh/Faulted/TEST10K_FLT_LGR_NNC.grdecl");
    //Mesh* MainMesh = MeshFactory::makeMesh("E:/GitLabRepository/PArallel MEsh LibrAry/examples/ECLIPSE Mesh/SPE9/SPE9.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:\GitLabRepository\PArallel MEsh LibrAry\examples/ECLIPSE Mesh/OW2P-MUFITS/OW2P-RT.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh("E:\GitLabRepository\PArallel MEsh LibrAry\examples/INRIA Unstructured Mesh/spe10_one_layer.mesh");


	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);


	////-------------------------Output
	MeshDataWriter* OutputWriter = new EnsightGoldWriter(MainMesh, "EclipseGridExample");

	//Variable declarations
	
	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");

	auto mesh_props = MainMesh->get_PolyhedronProperty()->get_PropertyMap();
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, it->first);
	}

	//Init
	OutputWriter->Init();

	//Set
	OutputWriter->SetVariable("Partition", Communicator::worldRank());
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->SetVariable(it->first, it->second);
	}

	//Dump
	OutputWriter->DumpVariables();

	Communicator::finalize();

	return 0;
}
