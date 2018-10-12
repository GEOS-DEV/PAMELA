#include "PAMELA.hpp"
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#ifdef WITH_VTK
#include <vtkMultiProcessController.h>
#include <vtkMPIController.h>
#endif

int main(int argc, char **argv) {

	using namespace  PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();
#ifdef WITH_VTK
	vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
	controler->Initialize(&argc, &argv, true);
	vtkMultiProcessController::SetGlobalController(controler.Get());
#endif

	//Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/gmsh/modelA1_volume_meshed.msh");
	Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/gmsh/mandaros.msh");

	MainMesh->CreateFacesFromCells();
	MainMesh->PerformPolyhedronPartitioning(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYGON);
	MainMesh->CreateLineGroupWithAdjacency("TopologicalC2C", MainMesh->getAdjacencySet()->get_TopologicalAdjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON));

	MeshDataWriter* OutputWriter = MeshDataWriterFactory::makeWriter(MainMesh, "UnstructuredGrid.vtm");

	OutputWriter->DeclareVariable(FAMILY::POLYHEDRON, VARIABLE_DIMENSION::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareVariable(FAMILY::POLYGON, VARIABLE_DIMENSION::SCALAR, VARIABLE_LOCATION::PER_CELL, "Partition");
	OutputWriter->DeclareAndSetElementGlobalIndex();
	OutputWriter->DeclareAndSetPartitionNumber();
	OutputWriter->Init();
	OutputWriter->Dump();
	Communicator::finalize();

	return 0;
}
