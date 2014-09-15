/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <stdexcept>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>
#include <wrap/io_trimesh/import_obj.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/export_vmi.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <mpi.h>

template <typename IOModule>
void check(int err) {
    if(err) {
        const char *msg = IOModule::ErrorMsg(err);
        throw std::runtime_error(msg);
    }
}

// in ImporterOBJ odd numbers are non-critical errors
template <>
void check<vcg::tri::io::ImporterOBJ<Mesh> >(int err) {
    typedef vcg::tri::io::ImporterOBJ<Mesh> IOModule;

    if( err == IOModule::OBJError::E_NON_CRITICAL_ERROR ||
        err == IOModule::OBJError::E_NOERROR ) return;
    if( err == IOModule::OBJError::E_NO_VERTEX ||
        err == IOModule::OBJError::E_NO_FACE ) {
        std::cerr << "WARNING: " << IOModule::ErrorMsg(err) << std::endl;
        return;
    }
    const char *msg = IOModule::ErrorMsg(err);
    throw std::runtime_error(msg);
}

void Mesh::readFileOBJ(const char * filename) {
    typedef vcg::tri::io::ImporterOBJ<Mesh> IOModule;
    IOModule::Info i;
    check<IOModule>(IOModule::LoadMask(filename, i));
    check<IOModule>(IOModule::Open(*this, filename, i));
}

void Mesh::readFileVMI(const char * filename) {
    typedef vcg::tri::io::ImporterVMI<Mesh> IOModule;
    int mask = 0;
    check<IOModule>(IOModule::Open(*this, filename, mask));
}

void Mesh::writeFileOBJ(const char * filename) {
    typedef vcg::tri::io::ExporterOBJ<Mesh> IOModule;
    check<IOModule>(IOModule::Save(*this, filename, 0));
}

void Mesh::writeFileVMI(const char * filename) {
    typedef vcg::tri::io::ExporterVMI<Mesh> IOModule;
    check<IOModule>(IOModule::Save(*this, filename));
}

void Mesh::dump(int* size, void** memptr) {
    typedef vcg::tri::io::ExporterVMI<Mesh> IOModule;
    int len = IOModule::BufferSize(*this);
    char *mem = new char[len];
    check<IOModule>(IOModule::DumpToMem(*this, mem));
    *size   = len;
    *memptr = mem;
}

void Mesh::read(void* mem) {
    typedef vcg::tri::io::ImporterVMI<Mesh> IOModule;
    int mask = 0;
    check<IOModule>(IOModule::ReadFromMem(*this, mask, (char *)mem));
}


void Mesh::send(int rank, int tag) {
    int size;
    char * mem;
    unique_ptr<char> memptr;

    this->dump(&size, (void**) &mem);
    memptr.reset(mem);

    MPI_Send( &size, 1, MPI_INT, rank, tag, MPI_COMM_WORLD);
    MPI_Send( memptr.get(), size, MPI_CHAR, rank, tag, MPI_COMM_WORLD);
}

void Mesh::recv(int rank, int tag) {
    int size;
    MPI_Recv(&size, 1, MPI_INT, rank, tag, MPI_COMM_WORLD, nullptr);

    vector<char> recvBuf( size );
    MPI_Recv(recvBuf.data(), size, MPI_CHAR, rank, tag, MPI_COMM_WORLD, nullptr);
    this->read( recvBuf.data() );
}



typedef vcg::tri::BasicVertexPair<Vertex> VertexPair;
class MyTriEdgeCollapse
    : public vcg::tri::TriEdgeCollapseQuadric<Mesh,
                                              VertexPair,
                                              MyTriEdgeCollapse,
                                              vcg::tri::QInfoStandard<Vertex> >
{
public:
    typedef  vcg::tri::TriEdgeCollapseQuadric<Mesh,
                                              VertexPair,
                                              MyTriEdgeCollapse,
                                              vcg::tri::QInfoStandard<Vertex> > TECQ;
    typedef  Mesh::VertexType::EdgeType EdgeType;
    inline MyTriEdgeCollapse(const VertexPair &p, int i, vcg::BaseParameterClass *pp)
        : TECQ(p,i,pp){}
};

void Mesh::merge( Mesh& other ) {
    vcg::tri::Append<Mesh,Mesh>::Mesh(*this, other);
    vcg::tri::Clean<Mesh>::MergeCloseVertex(*this, 1.0e-7);
}

void Mesh::simplify(int target_faces, bool preserve_boundary) {
    // take into account, that the boundary faces cannot be refined
    if(preserve_boundary) {
        Mesh::VertexIterator vi;
        int bv = 0;
        for(vi = this->vert.begin(); vi != this->vert.end(); ++vi) {
            if(vi->IsD()) continue;
            if(vi->IsB()) ++bv;
        }
        target_faces += bv;
    }
    if(target_faces > this->FN()) return;

    vcg::tri::TriEdgeCollapseQuadricParameter params;
    params.PreserveBoundary     = preserve_boundary;
    params.BoundaryWeight       = 999;

    vcg::LocalOptimization<Mesh> collapse(*this, &params);
    collapse.Init<MyTriEdgeCollapse>();

    collapse.SetTargetSimplices(target_faces);
    collapse.SetTimeBudget(0.5f);
    while(collapse.DoOptimization() && this->FN() > target_faces) {};
    collapse.Finalize<MyTriEdgeCollapse>();

    vcg::tri::Allocator<Mesh>::CompactFaceVector(*this);
    vcg::tri::Allocator<Mesh>::CompactVertexVector(*this);
}
