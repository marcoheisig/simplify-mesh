/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <stdexcept>
#include <vcg/complex/algorithms/clean.h>

void Mesh::merge(Mesh& other, float tolerance) {
    vcg::tri::Append<Mesh,Mesh>::Mesh(*this, other);
    vcg::tri::Clean<Mesh>::MergeCloseVertex(*this, tolerance);
}
