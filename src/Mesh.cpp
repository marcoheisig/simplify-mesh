/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Mesh.hpp"
#include <stdexcept>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>
#include <wrap/io_trimesh/import_obj.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/export_vmi.h>

void Mesh::readFileOBJ(std::string filename) {
    Mesh& mesh = *this;

    vcg::tri::io::ImporterOBJ<Mesh>::Info i;

    if (!vcg::tri::io::ImporterOBJ<Mesh>::LoadMask(filename.c_str(), i)) {
        throw std::runtime_error("failed to load mask during OBJ-file import");
    }
    vcg::tri::io::ImporterOBJ<Mesh>::Open(mesh, filename.c_str(), i);
}

void Mesh::writeFileOBJ(std::string filename) {
    vcg::tri::io::ExporterOBJ<Mesh>::Save(*this, filename.c_str(), 0);
}
void Mesh::writeFileVMI(std::string filename) {
    vcg::tri::io::ExporterVMI<Mesh>::Save(*this, filename.c_str());
}

void Mesh::dump(int* size, char** memptr) {
    int len = vcg::tri::io::ExporterVMI<Mesh>::BufferSize(*this);
    char *mem = new char[len];
    vcg::tri::io::ExporterVMI<Mesh>::DumpToMem(*this, mem);
    *size   = len;
    *memptr = mem;
}

void Mesh::read(char* mem) {
    int mask = 0;
    vcg::tri::io::ImporterVMI<Mesh>::ReadFromMem(*this, mask, mem);
}
