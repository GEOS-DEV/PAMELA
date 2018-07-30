#pragma once
// Library includes
#include "MeshDataWriters/MeshDataWriter.hpp"
#include <vtkXMLPMultiBlockDataWriter.h>
#include <vtkSmartPointer.h>

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

            /*!
             * @brief Declare the variables which are on the Mesh on the
             * VTK object
             */
            void DeclareAllVariables();

        private:
            /// VTU file
            vtkSmartPointer<vtkXMLPMultiBlockDataWriter> vtm_ {vtkXMLPMultiBlockDataWriter::New()};
    };
}
