/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#pragma once
#include <string>
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
                                std::vector<Edge> > {};

void createMeshFromOBJ(std::string filename);
