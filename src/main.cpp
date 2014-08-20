/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#include "App.hpp"
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    App app (&argc, &argv);
    app.run();
    MPI_Finalize();
}
