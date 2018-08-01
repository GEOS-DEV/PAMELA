
#include "MeshDataWriters/VTKWriter.hpp" 
#include "Mesh/Mesh.hpp"
#include "MeshDataWriters/Variable.hpp"
#include "Utils/Communicator.hpp"
#include "Utils/Logger.hpp"

#include <vtkBlockIdScalars.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkInformation.h>
#include <vtkCellData.h>
#include <vtkAOSDataArrayTemplate.h>
#include <vtkDoubleArray.h>

#include <iomanip>
namespace PAMELA
{
    /// -------------- PUBLIC METHODS
    void VTKWriter::Init() {
        DeclareAllVariables();
        MakeChildFiles();
        MakeParentFile();
        LOGINFO("number of blocks: " + std::to_string(block_->GetNumberOfBlocks()));
    }

    void VTKWriter::DumpVariables() {
    }
    
    /// -------------- PRIVATE METHODS
    
    void VTKWriter::DeclareAllVariables() {
        
    }

    template<typename T>
    void VTKWriter::MakeChildFile(const PartMap<T>* parts, const std::string& prefix) {

        int part = 0;
        for (auto it = parts->begin();
                it != parts->end();
                ++it) {
            block_->SetNumberOfBlocks(block_->GetNumberOfBlocks()+1);
            LOGINFO(std::to_string(part));
            vtkSmartPointer<vtkUnstructuredGrid> ug = vtkUnstructuredGrid::New();
            vtkSmartPointer<vtkPoints> vertices = vtkPoints::New();
            auto partptr = it->second;
            for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2) {
                double point[3] = 
                {(*it2)->get_coordinates().x, (*it2)->get_coordinates().y,
                    (*it2)->get_coordinates().z};
                vertices->InsertNextPoint(point);
            }
            ug->SetPoints(vertices);
            for (auto it2 = partptr->SubParts.begin();
                    it2 != partptr->SubParts.end(); ++it2) {
                if (it2->second->SubCollection.size_owned() > 0) {
                    auto subpart = it2->second;
                    auto elementType = subpart->ElementType;
                    auto vtkTypeLabel = ElementToLabel.at(elementType);
                    std::vector<int> cell_types;
                    int cell_index = 0 ;
                    vtkSmartPointer<vtkCellArray> vtkcells= vtkCellArray::New();
                    for (auto it3 = subpart->SubCollection.begin_owned(); it3 != subpart->SubCollection.end_owned(); ++it3)
                    {
                        cell_types.emplace_back(vtkTypeLabel);
                        auto VertexList = (*it3)->get_vertexList();
                        auto nVertex = VertexList.size();
                        vtkIdType corners[nVertex];
                        for (auto j = 0; j != nVertex; ++j)
                        {
                            corners[j] = partptr->GlobalToLocalPointMapping.at(VertexList[j]->get_globalIndex());
                        }
                        vtkcells->InsertNextCell(nVertex,corners);
                    }
                    ug->SetCells(cell_types.data(),vtkcells);
                }
                block_->SetBlock(block_->GetNumberOfBlocks()-1,ug);
            }
            vtkSmartPointer< vtkCellData > cell_data= ug->GetCellData();
            for (auto it2 = partptr->PerElementVariable.begin(); it2 != partptr->PerElementVariable.end(); ++it2)
            {
                auto variableptr = (*it2);
                for (auto it3 = partptr->SubParts.begin(); it3 != partptr->SubParts.end(); ++it3)
                {
                    if (it3->second->SubCollection.size_owned() > 0)
                    {
                        auto subpart = it3->second;
                        auto elementType = subpart->ElementType;
                        for (auto it4 = subpart->SubCollection.begin_owned(); it4 != subpart->SubCollection.end_owned(); ++it4)
                        {
                            auto dataarray = vtkDoubleArray::New();
                            auto collectionIndex = it4 - subpart->SubCollection.begin_owned();
                            auto variableIndex = subpart->IndexMapping[collectionIndex];
                            auto variableData = variableptr->get_data(variableIndex);
                            for (auto it5 = variableData.begin(); it5 != variableData.end(); ++it5)
                            {
                                dataarray->InsertNextTuple1(*it5);
                            }
                        }

                    }
                }

                /*
                vtkSmartPointer<vtkDataArray> data_array;
                if( variableptr->dType == VARIABLE_TYPE::SCALAR){
                }
                else if( variableptr->dType == VARIABLE_TYPE::VECTOR) {
                }
                else if( variableptr->dType == VARIABLE_TYPE::TENSOR_SYMM ) {
                }
                else {
                    continue;
                }
                */
                data_array->SetName(variableptr->Label.c_str());
                auto type = variableptr->dType;
                
                //cell_data->Add;
            }
        }


    }
    void VTKWriter::MakeChildFiles() {
        //-- Line
        MakeChildFile(&m_LineParts, "edge_");

        //-- Polygon
        MakeChildFile(&m_PolygonParts, "polygons_");

        //-- Polyhedron
        MakeChildFile(&m_PolyhedronParts, "polyhedron_");
    }

    void VTKWriter::MakeParentFile() {
        vtkSmartPointer<vtkMultiBlockDataSet> multi_block= 
            vtkMultiBlockDataSet::New();
        multi_block->SetNumberOfBlocks(Communicator::worldSize());
        auto controler =
            vtkMPIController::GetGlobalController();
        if( Communicator::worldRank() != 0 ) {
            controler->Send(block_,0,Communicator::worldRank());
        }
        else {
            multi_block->SetBlock(0, block_);
            LOGINFO("SET BLOCK 0");
            for(int proc = 1 ; proc < Communicator::worldSize(); proc++) {
                vtkSmartPointer< vtkMultiBlockDataSet> block = vtkMultiBlockDataSet::New();
                LOGINFO("nb of sous blocs " + std::to_string(block->GetNumberOfBlocks() ));
                controler->Receive(block,proc,proc);
                     multi_block->SetBlock(proc, block);
                LOGINFO("nb of sous blocs " + std::to_string(block->GetNumberOfBlocks() ));
            LOGINFO("SET BLOCK " + std::to_string(proc));
            }
        }
        if(Communicator::worldRank() == 0 ) {
            std::string filename = m_name + ".vtm";
            vtkSmartPointer< vtkXMLMultiBlockDataWriter> write =
                vtkXMLMultiBlockDataWriter::New();
            write->SetInputData(multi_block);
            write->SetFileName(filename.c_str());
            write->Write();
        }

        /*
            LOGINFO("t la");
            std::string filename = m_name + ".vtm";
            vtm_->SetInputData(multi_block);
            vtm_->SetFileName(filename.c_str());
            vtm_->Write();
            LOGINFO("oui.");
            */


    }

}