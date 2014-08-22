/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <memory>
#include <wrap/io_trimesh/import.h>

std::shared_ptr<Mesh> createMeshFromObj(std::string filename) {
    auto p = std::make_shared<Mesh>();
    int loadmask = 0; // TODO no idea what I'm doing here
    vcg::tri::io::ImporterOBJ<Mesh>::Open(*p, filename.c_str(), loadmask);
    return p;
}
