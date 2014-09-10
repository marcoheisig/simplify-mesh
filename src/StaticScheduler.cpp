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

    
	if( rank < (int) filenames.size() - filesRead ) {
		Task t = { TASK_READ };
		t.read.filename = filenames[filesRead+rank].c_str();
		filesRead += num_procs;
		return t;
	}

    Task t = { TASK_DIE };
    return t; // TODO
}

