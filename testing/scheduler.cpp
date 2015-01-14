/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IO
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "config.hpp"
#include "StaticScheduler.hpp"


using namespace std;

char * argv[] = { (char*) "./scheduler", nullptr };
int argc = 1;

BOOST_AUTO_TEST_CASE( SCHEDULER )
{

    int file_count = 100;
    int proc_count = 23;

    vector<string> files(file_count);
    int n = 0;
    generate_n( begin(files), file_count, [&n](){
            return to_string(n++);
        });

    vector< vector<string>> proc_states;

    unique_ptr<Scheduler> s( new StaticScheduler( vector<string>(file_count, "dummy"),
                                                  proc_count));



}
