//
// Created by Brian Weir (https://github.com/bweir27) on 11/14/20.
//

#include "Process.h"

/**
 * Constructor
 *
 * @param name              - a sequence of non-blank characters representing the name of the process
 * @param priority          - the priority level for the process, from 0 to 9
 * @param arrival_time      - the time at which the process arrives in the system
 * @param total_time        - the total amount of CPU time which will be used by the process
 * @param block_interval    - the interval at which the process will block for I/O.
 */
Process::Process(string name, uint32_t priority, uint32_t arrival_time, uint32_t total_time, uint32_t block_interval) {
    //set main private instance variable values
    this->p_name = name;
    this->priority = priority;
    this->arrival_time = arrival_time;
    this->total_time = total_time;
    this->block_interval = block_interval;

    this->time_until_arrival = arrival_time;

    //initalize the block_time_remaining to 0 to allow a process to start immediately for the first time
    this->block_time_remaining = 0;
    this->time_until_next_block = this->block_interval;
    this->time_until_finished = this->total_time;

    //initialize times to invalid values to help with debugging
    this->start_time = arrival_time;
    this->finish_time = -1;
    this->turnaround_time = -1;
    this->is_finished = false;
}

/**
 * print    -   used mostly for debugging, prints the data points for this process
 */
void Process::print() const{
    cout<<"("<< this->p_name << " "
        << this->priority << " "
        << this->arrival_time << " "
        << this->total_time << " "
        << this->block_interval << " | "
        << "btr: "<<this->block_time_remaining << " "
        << "tuf: "<<this->time_until_finished<< " "
        << "isFin: "<<this->is_finished<< " "
        << "tunb: "<<this->time_until_next_block << " "
        << "tua: " << this->time_until_arrival << " | "
        << "start: "<<this->start_time << " "
        <<"fin: "<<this->finish_time<<" "
        <<"turn: "<<this->turnaround_time<<")";
}
