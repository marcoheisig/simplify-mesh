/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "StaticScheduler.hpp"
#include <iostream>

using namespace std;

StaticScheduler::StaticScheduler(const std::vector<std::string>& filenames,
                                 size_t num_procs)
    : Scheduler(filenames),
      num_procs(num_procs) {
}

Task StaticScheduler::getTask(int rank, int iteration, const Mesh& mesh)  {


    Task t = { TASK_DIE };

    if( iteration == 0) {
        t = { TASK_READ };
        for( size_t i = rank; i < filenames.size(); i+= num_procs) {
            t.read.filenames.push_back(filenames[i]);
        }
        return t;
    }


    if( rank % (int) pow(2, iteration) == 0 ) {
        if( rank+pow(2, iteration-1) < num_procs ) {
            t = {TASK_RECEIVE};
            t.receive.mpi_tag = iteration;
            t.receive.mpi_rank = rank+pow(2, iteration-1);
        } else {
            if( rank == 0) {
                t = {TASK_WRITE};
            } else  {
                t = {TASK_IDLE};
            }
        }
    } else  if( rank % (int) pow(2, iteration-1) == 0 ) {
        t = {TASK_SEND};
        t.send.mpi_tag = iteration;
        t.send.mpi_rank = rank-pow(2, iteration-1);
    }


    return t;
}

