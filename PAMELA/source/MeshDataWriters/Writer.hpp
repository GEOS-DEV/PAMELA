#pragma once
// Library includes
#include "MeshDataWriters/MeshDataWriter.hpp"
namespace PAMELA
{
    class Writer : public MeshDataWriter{
        public:
            Writer(Mesh * mesh, std::string name) : MeshDataWriter(mesh,name) {}
            virtual void Init() final {};
            virtual void Dump() final {};

            const PartMap<Polyhedron*> &  GetPolyhedronMap() const {
              return m_PolyhedronParts;
            }
    };
}
