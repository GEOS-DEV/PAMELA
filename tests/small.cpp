#include <iostream>
#include <fstream>

#include "Parallel/Communicator.hpp"
#include "test_io.h"

#ifdef WITH_VTK
#include<vtkSmartPointer.h>
#include<vtkMPIController.h>
#endif

int main(int argc, char **argv) {
    using namespace PAMELA;
    Communicator::initialize();
#ifdef WITH_VTK
    vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
    controler->Initialize(&argc,&argv,true);
    vtkMultiProcessController::SetGlobalController(controler.Get());
#endif

    std::ifstream file_list(std::string(PAMELA_PATH) + "/tests/small.txt");
    load_and_save(file_list);

}
