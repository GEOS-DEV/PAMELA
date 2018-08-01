#pragma once
// Library includes
#include "MeshDataWriters/MeshDataWriter.hpp"

#ifdef WITH_VTK
#include <vtkXMLPMultiBlockDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkMPIController.h>
#include <vtkMultiBlockDataSet.h>

#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif


namespace PAMELA
{
    class VTKWriter : public MeshDataWriter{
        public:
            VTKWriter(Mesh * mesh, std::string name) : MeshDataWriter(mesh,name) {}
            virtual void Init() final;
            virtual void DumpVariables() final;

        private:

            /*!
             * @brief Make the parent file (.pvtu)
             */
            void MakeParentFile();

            /*!
             * @brief Make the children files (.vtu)
             */
            void MakeChildFiles();

	    template<typename T>
            void MakeChildFile(const PartMap<T>* parts, const std::string& prefix);
            /*!
             * @brief Declare the variables which are on the Mesh on the
             * VTK object
             */
            void DeclareAllVariables();

        private:
            /// VTU file
            vtkSmartPointer<vtkXMLPMultiBlockDataWriter> vtm_ {vtkXMLPMultiBlockDataWriter::New()};
            vtkSmartPointer<vtkMultiBlockDataSet> block_ {vtkMultiBlockDataSet::New()};

            const std::unordered_map<ELEMENTS::TYPE, int> ElementToLabel
                =
                {
                    { ELEMENTS::TYPE::VTK_VERTEX, 1 },
                    { ELEMENTS::TYPE::VTK_LINE, 3  },
                    { ELEMENTS::TYPE::VTK_TRIANGLE,5 },
                    { ELEMENTS::TYPE::VTK_QUAD ,9 },
                    { ELEMENTS::TYPE::VTK_TETRA,10 },
                    { ELEMENTS::TYPE::VTK_HEXAHEDRON , 12 },
                    { ELEMENTS::TYPE::VTK_WEDGE, 13 },
                    { ELEMENTS::TYPE::VTK_PYRAMID,14 }
                };
            /// MPI controller for vtk
            //vtkSmartPointer<vtkMPIController> vtk_controller_ {vtkMPIController::New()};
    };
}
#endif
