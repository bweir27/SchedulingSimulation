//
// Created by Brian Weir (https://github.com/bweir27) on 11/14/20.
//

#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H
#include "Process.h"


class RoundRobin {

    std::string file_name;  //the name of the input file
    uint32_t block_duration;    // the time each process blocks before it can execute again
    uint32_t time_slice;    // the time allotted to each process by the Scheduler before it is forced to block

    uint32_t maxPriority; //to be used to determine the number of qTable indices needed
    std::vector<Process> allProcesses; //the list of ALL processes

    std::vector<std::vector<Process>> qTable; //the queue table
    std::vector<Process> blockedProcesses; //the list of processes that are currently blocked
    std::vector<Process> unarrived_processes; //the list of processes that haven't arrived yet (according to the clock)
    std::vector<Process> finished_processes; //the list of processes that have completed execution
    uint32_t numTotalProcesses; // the total number of processes that need to be performed

    uint32_t clock; //used to maintain the clock in the program
    uint32_t interval; //used to track how long the most current idle/execution interval is
public:
    RoundRobin(std::string fileName, uint32_t block, uint32_t slice); //constructor
    RoundRobin(const RoundRobin& other) = delete;   //copy constructor
    RoundRobin(RoundRobin&& other) = delete;                 //move constructor
    RoundRobin& operator=(const RoundRobin& other) = delete;    //copy assignment
    RoundRobin& operator=(RoundRobin&& other) noexcept = delete; //move assignment

    void read_processes();  //reads in the list of processes from the file
    void initQTable();  //initializes the queue table

private:
    void perform_processes(); //executes the processes
    void update_QTable();
    double calc_avg_turnaround();   //calculates the average turnaround time of all processes

    //rest of these were used mostly for debugging purposes
    void print_all_processes();
    void print_all_blocked();
    void print_all_unarrived();
    void print_all_finished();
    void printQTable();
};


#endif //ROUNDROBIN_H
