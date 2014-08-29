/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include <string>

enum {
    TASK_DIE,
    TASK_SEND,
    TASK_RECEIVE,
    TASK_READ
};

struct Task_die {
};

struct Task_send {
    int mpi_rank;
    int mpi_tag;
};

struct Task_receive {
    int mpi_rank;
    int mpi_tag;
    int size;
};

struct Task_read {
    char *filename;
};

struct Task {
    int type;
    union {
        Task_die     die;
        Task_send    send;
        Task_receive receive;
        Task_read    read;
    };
};
