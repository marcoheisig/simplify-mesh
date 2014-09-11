/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "StaticScheduler.hpp"
#include <iostream>

using namespace std;

StaticScheduler::StaticScheduler(const std::vector<std::string>& filenames,
                                 size_t num_procs)
    : Scheduler(filenames),
      num_procs(num_procs),
      iteration(0),
      filesRead(0) {
}

Task StaticScheduler::getTask(int rank, const Mesh& mesh) {


    Task t = { TASK_DIE };

    if( rank < (int) filenames.size() - filesRead ) {
        t = { TASK_READ };
        t.read.filename = filenames[filesRead+rank].c_str();
        filesRead += num_procs;
        return t;
    }



    if( rank % (int) pow(2, iteration+1) == 0 ) {
        if( rank+pow(2, iteration) < num_procs ) {
            t = {TASK_RECEIVE};
            t.receive.mpi_tag = iteration;
            t.receive.mpi_rank = rank+pow(2, iteration);
        } else {
            if( rank == 0) {
                t = {TASK_WRITE};
            } else  {
                t = {TASK_IDLE};
            }
        }
    }

    if( rank % (int) pow(2, iteration+1) == pow(2, iteration) ) {
        t = {TASK_SEND};
        t.receive.mpi_tag = iteration;
        t.receive.mpi_rank = rank-pow(2, iteration);
    }



    iteration++;
    return t;
}

