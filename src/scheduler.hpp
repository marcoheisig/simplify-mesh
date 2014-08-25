/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include "Mesh.hpp"
#include "Job.hpp"

Job getWork(int rank, const Mesh& mesh);
