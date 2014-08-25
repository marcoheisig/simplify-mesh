/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Process.hpp"
#include <iostream>
#include <exception>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    try {
        Process p (&argc, &argv);
        p.run();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "FATAL ERROR" << std::endl;
    }
    MPI_Finalize();
}
