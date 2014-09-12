/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#pragma once
#include <string>
#include <vector>

class StopProgram : public std::exception { };

class Process {
public:
    Process(int *argc, char ***argv);
    void run();
private:
    std::vector<std::string> infiles;
    std::string outfile;
    int  rank;
    int  num_procs;
    int  max_faces;
    int  target_faces;
    bool logging;
};
