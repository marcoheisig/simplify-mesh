/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include "Mesh.hpp"

enum jobs {
    GROW,
    YIELD,
    READ,
    DIE
};

int getWork(int rank, const Mesh& mesh, int& target_rank, int& tag);
