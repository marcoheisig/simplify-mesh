/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <wrap/io_trimesh/import.h>
#include "util.hpp"

std::shared_ptr<Mesh> createMeshFromOBJ(std::string filename) {
    auto p = std::make_shared<Mesh>();

    vcg::tri::io::ImporterOBJ<Mesh>::Info i;

    if (!vcg::tri::io::ImporterOBJ<Mesh>::LoadMask(filename.c_str(), i)) {
        THROW_EXCEPTION("failed to load mask during OBJ-file import");
    }

    vcg::tri::io::ImporterOBJ<Mesh>::Open(*p, filename.c_str(), i);
    return p;
}
