/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <stdexcept>
#include <wrap/io_trimesh/import.h>

std::shared_ptr<Mesh> createMeshFromOBJ(std::string filename) {
    auto p = std::make_shared<Mesh>();

    vcg::tri::io::ImporterOBJ<Mesh>::Info i;

    if (!vcg::tri::io::ImporterOBJ<Mesh>::LoadMask(filename.c_str(), i)) {
        throw runtime_error("failed to load mask during OBJ-file import");
    }

    vcg::tri::io::ImporterOBJ<Mesh>::Open(*p, filename.c_str(), i);
    return p;
}
