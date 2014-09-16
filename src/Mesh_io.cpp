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
