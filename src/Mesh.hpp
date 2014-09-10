/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include <string>
#include <memory>
#include <vcg/complex/complex.h>
#include <vcg/math/quadric.h>

class Vertex;
class Edge;
class Face;

struct UsedTypes
    : public vcg::UsedTypes< vcg::Use<Vertex> :: AsVertexType,
                             vcg::Use<Edge>   :: AsEdgeType,
                             vcg::Use<Face>   :: AsFaceType>{};

class Vertex
    : public vcg::Vertex< UsedTypes,
                          vcg::vertex::VFAdj,
                          vcg::vertex::Coord3f,
                          vcg::vertex::Normal3f,
                          vcg::vertex::Mark,
                          vcg::vertex::BitFlags >{
public:
    vcg::math::Quadric<double> &Qd() {return q;}
private:
    vcg::math::Quadric<double> q;
};

class Face
    : public vcg::Face< UsedTypes,
                        vcg::face::FFAdj,
                        vcg::face::VFAdj,
                        vcg::face::VertexRef,
                        vcg::face::BitFlags > {};

class Edge
    : public vcg::Edge< UsedTypes> {};

class Mesh
    : public vcg::tri::TriMesh< std::vector<Vertex>,
                                std::vector<Face> > {
public:
    void readFileOBJ(const char * filename);
    void readFileVMI(const char * filename);
    void writeFileOBJ(const char * filename);
    void writeFileVMI(const char * filename);
    void dump(int* size, void** memptr);
    void read(void* mem);
	void recv(int rank, int tag);
	void send(int rank, int tag);

    void simplify(int target_faces);
	void merge( Mesh & other );
};
