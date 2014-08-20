/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#include <iostream>
#include <boost/program_options.hpp>
#include <mpi.h>
#include "App.hpp"
#include "local_coarsening.hpp"
#include "merging.hpp"
#include "Mesh.hpp"

App::App(int *argc, char ***argv) {

    namespace po = boost::program_options;
    po::options_description desc("options");
    desc.add_options()
        ("help", "produce help message")
        ("version", "display version information")
        ("size,s", po::value<int>()->default_value(50), "set max size in MB")
        // ...
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(*argc, *argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return;
    }

    if (vm.count("version")) {
        std::cout << "distributed-polygon-reducer v" << 0 << std::endl; // TODO
        return;
    }

    if (vm.count("size")) {
        std::cout << "max size was set to "
             << vm["size"].as<int>() << ".\n";
    };

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // TODO std::string filename = foobar
}

void App::run() {
    // TODO
}

