/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <stdexcept>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

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
