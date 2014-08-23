/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#include "App.hpp"
#include <iostream>
#include <exception>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    try {
        App app (&argc, &argv);
        app.run();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "FATAL ERROR" << std::endl;
    }
    MPI_Finalize();
}
