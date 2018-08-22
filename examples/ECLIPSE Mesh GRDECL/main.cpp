#include "PAMELA.hpp"
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Adjacency/Adjacency.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#include <vtkMultiProcessController.h>
#include <vtkMPIController.h>

int main(int argc, char **argv) {

	using namespace  PAMELA;


	Communicator::initialize();
#ifdef WITH_VTK
	vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
	controler->Initialize(&argc, &argv, true);
	vtkMultiProcessController::SetGlobalController(controler.Get());
#endif

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Johansen/FULLFIELD_IMAXJMAX.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Norne/GRID/IRAP_1005.GRDECL");
	Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Dalia/grid.GRDECL");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	////-------------------------Output
	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "EclipseGrid.vtm");

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
	for (auto& mesh_prop : mesh_props)
	{
		OutputWriter->SetVariable(mesh_prop.first, mesh_prop.second);
	}

	//Dump
	OutputWriter->Dump();

	Communicator::finalize();

	return 0;
}
