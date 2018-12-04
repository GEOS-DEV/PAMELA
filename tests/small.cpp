#include <iostream>
#include <fstream>

#include "Parallel/Communicator.hpp"
#include "test_io.h"
#include "gtest/gtest.h"

#ifdef WITH_VTK
#include<vtkSmartPointer.h>
#include<vtkMPIController.h>
#endif

using namespace PAMELA;

int main(int argc, char **argv) {
    Communicator::initialize();
    ::testing::InitGoogleTest(&argc, argv);
#ifdef WITH_VTK
    vtkSmartPointer<vtkMPIController> controler = vtkMPIController::New();
    controler->Initialize(&argc,&argv,true);
    vtkMultiProcessController::SetGlobalController(controler.Get());
#endif
  int const result = RUN_ALL_TESTS();
  Communicator::finalize();
  return result;
}

TEST(testSMALL,testSMALL)
{
    std::ifstream file_list(std::string(PAMELA_PATH) + "/tests/small.txt");
    load_and_save(file_list);
}
