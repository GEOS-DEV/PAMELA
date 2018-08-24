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
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/ReducedNorne/IRAP_1005.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/SPE9/SPE9_CP.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Sector/SECTOR.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Brugge/BRUGGE_0000.EGRID");
	Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Sector/SECTOR.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Skew_MPFAO/SKEWO.EGRID");


	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	////-------------------------Output
	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "EclipseGrid.vtm");

	//Variable declarations
	auto mesh_props = MainMesh->get_PolyhedronProperty()->get_PropertyMap();
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_TYPE::SCALAR, VARIABLE_LOCATION::PER_CELL, it->first);
	}
	//
	OutputWriter->DeclareAndSetElementGlobalIndex();
	OutputWriter->DeclareAndSetPartitionNumber();
	OutputWriter->DeclareAndSetAdjacency("Volume to Volume", MainMesh->getMeshAdjacency()->get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON));

	//Set
	for (auto& mesh_prop : mesh_props)
	{
		OutputWriter->SetVariableOnPolyhedron(mesh_prop.first, mesh_prop.second);
	}

	//Init
	OutputWriter->Init();


	//Dump
	OutputWriter->Dump();

	Communicator::finalize();

	return 0;
}
