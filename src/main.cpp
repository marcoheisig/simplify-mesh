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
    } catch (StopProgram&) {
        // NOP - just fall through to MPI_Finalize()
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR: something other than an exception was thrown" << std::endl;
    }
    MPI_Finalize();
}
