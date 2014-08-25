/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include "Mesh.hpp"
#include "Job.hpp"

class Scheduler {
protected:
    Scheduler(const std::vector<std::string>& filenames)
        : filenames(filenames) {}
public:
    virtual Job getWork(int rank, const Mesh& mesh) = 0;

    const std::vector<std::string> filenames;
};
