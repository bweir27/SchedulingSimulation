//
// Created by Brian Weir (https://github.com/bweir27) on 11/14/20.
//

#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <ios>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <iomanip>
#include <vector>
#include <queue>
#include <typeinfo>
#include <algorithm>

using namespace std;

class Process {

//    string p_name;
//    uint32_t priority;
//    uint32_t arrival_time;
//    uint32_t total_time;
//    uint32_t block_interval;
//    uint32_t block_time_remaining;
//    bool is_finished;
public:
    //the core behavior values
    string p_name;
    uint32_t priority;
    uint32_t arrival_time;
    uint32_t total_time;
    uint32_t block_interval;

    //other variables used
    uint32_t time_until_finished;
    uint32_t time_until_next_block;
    int time_until_arrival;
    int block_time_remaining;
    int start_time;
    int finish_time;
    double turnaround_time;
    bool is_finished;


    Process(string name, uint32_t priority, uint32_t arrival_time, uint32_t total_time, uint32_t block_interval); //constructor

//    friend bool operator< (const Process& p1, const Process& p2);

    void print();

private:


};


#endif //PROCESS_H
