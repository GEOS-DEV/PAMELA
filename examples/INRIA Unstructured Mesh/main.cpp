#include "PAMELA.hpp"
#include "Mesh/MeshFactory.hpp"
#include "Mesh/Mesh.hpp"
#include "Parallel/Communicator.hpp"
#include <thread>
#include "MeshDataWriters/MeshDataWriterFactory.hpp"
#include <vtkMultiProcessController.h>
#include <vtkMPIController.h>

int main(int argc, char **argv) {

	using namespace  PAMELA;

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	Communicator::initialize();
#ifdef WITH_VTK
	vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
	controler->Initialize(&argc, &argv, true);
	vtkMultiProcessController::SetGlobalController(controler.Get());
#endif


	Mesh* MainMesh = MeshFactory::makeMesh(PAMELA_PATH"/data/medit/small.mesh");

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

