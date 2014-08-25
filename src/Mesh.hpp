/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include <string>
#include <memory>
#include <vcg/complex/complex.h>

class Vertex;
class Edge;
class Face;

struct UsedTypes
    : public vcg::UsedTypes< vcg::Use<Vertex> :: AsVertexType,
                             vcg::Use<Edge>   :: AsEdgeType,
                             vcg::Use<Face>   :: AsFaceType>{};

class Vertex
    : public vcg::Vertex< UsedTypes,
                          vcg::vertex::Coord3f,
                          vcg::vertex::Normal3f,
                          vcg::vertex::BitFlags >{};

class Face
    : public vcg::Face< UsedTypes,
                        vcg::face::FFAdj,
                        vcg::face::VertexRef,
                        vcg::face::BitFlags > {};

class Edge
    : public vcg::Edge< UsedTypes> {};

class Mesh
    : public vcg::tri::TriMesh< std::vector<Vertex>,
                                std::vector<Face> ,
                                std::vector<Edge> > {
public:
    void  readFileOBJ(std::string filename);
    void writeFileOBJ(std::string filename);
    void writeFileVMI(std::string filename);
    void writeFile3DS(std::string filename);
    void dump(int* size, char** memptr);
    void read(char* mem);
};
