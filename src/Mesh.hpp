/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#pragma once
#include <string>
class Mesh;

Mesh createMeshFromObj(std::string filename);

/* try to determine the file type automatically */
Mesh createMeshFromFile(std::string filename);

class Mesh {
};
