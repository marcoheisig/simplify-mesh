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

template <typename IOModule>
void check(int err) {
    if(err) {
        const char *msg = IOModule::ErrorMsg(err);
        throw std::runtime_error(msg);
    }
}

/* in OBJ-import both 0 and 1 are ok */
template <>
void check<vcg::tri::io::ImporterOBJ<Mesh> >(int err) {
    typedef vcg::tri::io::ImporterOBJ<Mesh> IOModule;
    if(err != 0 && err != 1) {
        const char *msg = IOModule::ErrorMsg(err);
        throw std::runtime_error(msg);
    }
}

void Mesh::readFileOBJ(char * filename) {
    typedef vcg::tri::io::ImporterOBJ<Mesh> IOModule;
    IOModule::Info i;
    check<IOModule>(IOModule::LoadMask(filename, i));
    check<IOModule>(IOModule::Open(*this, filename, i));
}

void Mesh::readFileVMI(char * filename) {
    typedef vcg::tri::io::ImporterVMI<Mesh> IOModule;
    int mask = 0;
    check<IOModule>(IOModule::Open(*this, filename, mask));
}

void Mesh::writeFileOBJ(char * filename) {
    typedef vcg::tri::io::ExporterOBJ<Mesh> IOModule;
    check<IOModule>(IOModule::Save(*this, filename, 0));
}

void Mesh::writeFileVMI(char * filename) {
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

void Mesh::simplify(int target_faces) {
    vcg::tri::TriEdgeCollapseQuadricParameter params;
    params.FastPreserveBoundary = true;
    params.PreserveBoundary     = false;

    vcg::LocalOptimization<Mesh> collapse(*this, &params);
    collapse.Init<MyTriEdgeCollapse>();
    collapse.SetTargetSimplices(target_faces);
    while(collapse.DoOptimization() && this->FN() > target_faces) {};
    collapse.Finalize<MyTriEdgeCollapse>();
}
