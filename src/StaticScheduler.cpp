/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "StaticScheduler.hpp"

StaticScheduler::StaticScheduler(const std::vector<std::string>& filenames,
                                 size_t num_procs)
    : Scheduler(filenames),
      num_procs(num_procs) {
    
}

Job StaticScheduler::getWork(int rank, const Mesh& mesh) {
    
    return Die(); // TODO
}
