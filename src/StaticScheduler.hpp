/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include <vector>
#include <string>
#include "Scheduler.hpp"

class StaticScheduler : public Scheduler {
public:
    StaticScheduler(const std::vector<std::string>& filenames,
                    size_t num_procs);


    Job getWork(int rank, const Mesh& mesh);
private:
    const size_t num_procs;
};
