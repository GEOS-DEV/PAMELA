#include "PAMELA.hpp"
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#include <vtkMultiProcessController.h>
#include <vtkMPIController.h>
#include <ctime>

int main(int argc, char **argv) {

	using namespace  PAMELA;


	Communicator::initialize();
#ifdef WITH_VTK
	vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
	controler->Initialize(&argc, &argv, true);
	vtkMultiProcessController::SetGlobalController(controler.Get());
#endif

	time_t tstart;
	tstart = time(0);

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/ReducedNorne/IRAP_1005.GRDECL");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/SPE9/SPE9_CP.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Sector/SECTOR.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Brugge/BRUGGE_0000.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Sector/SECTOR.EGRID");
	Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Norne/GRID/NORNE_ATW2013.EGRID");
	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/eclipse/Skew_MPFAO/SKEWO.EGRID");


	MainMesh->CreateFacesFromCells();

	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);

	MainMesh->getAdjacencySet()->Add_NonTopologicalAdjacencySum("NNCs+PreProc", { MainMesh->getAdjacencySet()->get_NonTopologicalAdjacency("NNCs") ,MainMesh->getAdjacencySet()->get_NonTopologicalAdjacency("PreProc") });
	MainMesh->CreateLineGroupWithAdjacency("TopologicalC2C", MainMesh->getAdjacencySet()->get_TopologicalAdjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON));
	MainMesh->CreateLineGroupWithAdjacency("NNCs", MainMesh->getAdjacencySet()->get_NonTopologicalAdjacency("NNCs"));
	MainMesh->CreateLineGroupWithAdjacency("PreProc", MainMesh->getAdjacencySet()->get_NonTopologicalAdjacency("PreProc"));
	MainMesh->CreateLineGroupWithAdjacency("NNCs+PreProc", MainMesh->getAdjacencySet()->get_NonTopologicalAdjacency("NNCs+PreProc"));

	////-------------------------Output
	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "EclipseGrid.vtm");

	//Variable declarations
	auto mesh_props = MainMesh->get_PolyhedronProperty_double()->get_PropertyMap();
	for (auto it = mesh_props.begin(); it != mesh_props.end(); ++it)
	{
		OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_DIMENSION::SCALAR, VARIABLE_LOCATION::PER_CELL, it->first);
	}
	//
	OutputWriter->DeclareAndSetElementGlobalIndex();
	OutputWriter->DeclareAndSetPartitionNumber();

	//Init
	OutputWriter->Init();

	//Set
	for (auto& mesh_prop : mesh_props)
	{
		OutputWriter->SetVariableOnPolyhedron(mesh_prop.first, mesh_prop.second);
	}

	//Dump
	OutputWriter->Dump();

	time_t tend = time(0);
	LOGINFO("It took " + std::to_string(difftime(tend, tstart)) + " second(s).");

	delete MainMesh, OutputWriter;

	Communicator::finalize();

	return 0;
}
