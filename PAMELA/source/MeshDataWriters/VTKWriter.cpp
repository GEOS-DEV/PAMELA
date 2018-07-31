
#include "MeshDataWriters/VTKWriter.hpp" 
#include "Utils/Communicator.hpp"
#include "Utils/Logger.hpp"

#include <vtkMultiBlockDataSet.h>

#include <iomanip>
namespace PAMELA
{
    /// -------------- PUBLIC METHODS
    void VTKWriter::Init() {
        vtk_controller_->Initialize();
        vtkMultiProcessController::SetGlobalController(vtk_controller_.Get());
        DeclareAllVariables();
        MakeParentFile();
        MakeChildFiles();
    }

    void VTKWriter::DumpVariables() {
    }
    
    /// -------------- PRIVATE METHODS
    
    void DeclareAllVariables() {
        
    }

    void VTKWriter::MakeParentFile() {
        vtkSmartPointer<vtkMultiBlockDataSet> a_block= 
            vtkMultiBlockDataSet::New();

        vtm_->AddInputDataObject(a_block);
        vtm_->Write();

    }

}
