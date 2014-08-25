/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include <string>

struct Job {
    enum type {
        GROW,
        YIELD,
        READ,
        DIE
    };
    const int type;
protected:
    Job(int type) : type(type) {}
};

struct Die : public Job {
    Die() : Job(DIE) {}
};

struct Grow : public Job {
    Grow(int target_rank, int mpi_tag)
        : Job(GROW),
          target_rank(target_rank),
          mpi_tag(mpi_tag)
    {}
    int target_rank;
    int mpi_tag;
};

struct Yield : public Job {
    Yield(int target_rank, int mpi_tag)
        : Job(YIELD),
          target_rank(target_rank),
          mpi_tag(mpi_tag)
    {}
    int target_rank;
    int mpi_tag;
};

struct Read : public Job {
    Read(std::string filename)
        : Job(READ),
          filename(filename)
    {}
    std::string filename;
};
