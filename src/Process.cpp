/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Process.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include <mpi.h>
#include "StaticScheduler.hpp"
#include "Mesh.hpp"

using namespace std;

Process::Process(int *argc, char ***argv) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    namespace po = boost::program_options;
    po::options_description desc("options");
    desc.add_options()
        ("help", "produce help message")
        ("version", "display version information")
        ("size,s", po::value<int>()->default_value(5000), "set target number of faces")
        ("input",  po::value<std::vector<std::string> >(), "input files")
        ("output,o", po::value<std::string>(), "output file")
        // ...
        ;

    po::positional_options_description p;
    p.add("input", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(*argc, *argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (rank != 0) return;

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return;
    }

    if (vm.count("version")) {
        std::cout << "distributed-polygon-reducer v" << 0 << std::endl; // TODO
        return;
    }

    std::string outfile;
    std::vector<std::string> infiles;
    if(vm.count("input") && vm.count("output")) {
        infiles = vm["input" ].as<std::vector<std::string> >();
        outfile = vm["output"].as<std::string>();
    } else {
        throw std::runtime_error("no input or output file specified");
    }

    if (infiles.size() == 1) {
        std::cout << "converting from file " << infiles[0]
                  << " to "                  << outfile
                  << "." << std::endl;
    } else {
        std::cout << "Merging " << infiles.size() << " files into "
                  << outfile << "." << std::endl;
    }

    if (vm.count("size")) {
        std::cout << "target mesh size was set to "
                  << vm["size"].as<int>() << " facess.\n";
        this->target_faces = vm["size"].as<int>();
    } else {
        throw std::runtime_error("no target number of faces specified");
    };
}

void Process::run() {

    Scheduler* scheduler = new StaticScheduler({{"test4"},{"test4"}, {"test1"}, {"test2"}, {"test3"}}, num_procs);
    Mesh mesh;
    bool alive = true;
    while(alive) {
        Task task = scheduler->getTask(rank, mesh);
        switch( task.type ) {
        case TASK_RECEIVE:
            {
				Mesh recvMesh;
				recvMesh.recv( task.receive.mpi_rank, task.receive.mpi_tag );
				mesh.merge(recvMesh);
            }
            break;
        case TASK_SEND:
            {
				mesh.send( task.send.mpi_rank, task.send.mpi_tag );
            }
            break;
        case TASK_READ:
            {
				/*	Mesh new_mesh;
				new_mesh.readFileOBJ( task.read.filename );
				mesh.merge( new_mesh );*/
				std::cout << rank << " reads file \"" << task.read.filename << "\"\n";
            }
            break;
        case TASK_DIE:
            alive = false;
            break;
        default:
            throw std::runtime_error("invalid task type");
            break;
        }
    }
}

