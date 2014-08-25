/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Process.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <mpi.h>
#include "StaticScheduler.hpp"
#include "Mesh.hpp"

Process::Process(int *argc, char ***argv) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    namespace po = boost::program_options;
    po::options_description desc("options");
    desc.add_options()
        ("help", "produce help message")
        ("version", "display version information")
        ("size,s", po::value<int>()->default_value(50), "set max size in MB")
        ("input", "input file")
        ("output", "output file")
        // ...
        ;

    po::positional_options_description p;
    p.add("input", 1);
    p.add("output", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(*argc, *argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return;
    }

    if (vm.count("version")) {
        std::cout << "distributed-polygon-reducer v" << 0 << std::endl; // TODO
        return;
    }

    if(vm.count("input") && vm.count("output")) {
        input = vm["input" ].as<std::string>();
        output = vm["output"].as<std::string>();
        std::cout << "converting from file " << input
                  << " to "                  << output
                  << "." << std::endl;
    } else {
        throw std::runtime_error("no input or output file specified");
    }

    if (vm.count("size")) {
        std::cout << "target mesh size was set to "
                  << vm["size"].as<int>() << " MB.\n";
    };
}

void Process::run() {
    Scheduler* scheduler = new StaticScheduler({{"test1"}, {"test2"}}, 1);
    Mesh mesh;
    mesh.readFileOBJ(input);
    std::cout << "Imported a mesh with " << mesh.VN() << " vertices" << std::endl;
    mesh.writeFileOBJ(output);

    char  *mem;
    int size;
    mesh.dump(&size, &mem);
    std::cout << size / 1000000 << " MB of mesh dumped to memory" << std::endl;
    Mesh mesh2;
    mesh2.read(mem);
    std::cout << "mesh retrieved from memory" << std::endl;

    std::cout << "mesh has " << mesh2.VN() << " vertices after reimport" << std::endl;

    bool alive = true;
    while( alive ) {
        Job job = scheduler->getWork(rank, mesh);
        switch( job.type ) {
        case Job::GROW:
            // receive
            // merge
            // coarsen
            break;
        case Job::YIELD:
            // send
            break;
        case Job::READ:
            // read from file
            // coarsen
            break;
        case Job::DIE:
            alive = false;
            break;
        default:
            throw std::runtime_error("invalid job");
            break;
        }
    }
}

