/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */
#pragma once
#include <string>

class App {
public:
    App(int *argc, char ***argv);
    void run();
private:
    int rank;
    std::string input;
    std::string output;
};