//
// Created by Brian Weir (https://github.com/bweir27) on 11/14/20.
//

#include "RoundRobin.h"

using namespace std;

FILE *toRead;
ifstream inFile;

/**
 * Constructor
 *
 * @param fileName  - the name of the .txt file containing the information needed by the scheduler
 * @param block     - the the decimal integer time length that a process is unavailable to run after it blocks
 * @param slice     - the decimal integer length of the time slice for the Round-Robin scheduler
 */
RoundRobin::RoundRobin(string fileName, uint32_t block, uint32_t slice) {
    this->file_name = fileName;
    this->block_duration = block;
    this->time_slice = slice;

    file_name = fileName;
    const char* c_string_fileName= file_name.c_str();

    try{
        //handle errors opening the file
        inFile.open(fileName);
        if(inFile.fail()) {
            char e[100] = "File not found: ";
            throw runtime_error(strcat(e, c_string_fileName));
        }
        toRead = fopen(c_string_fileName, "r+");
        if(toRead == nullptr){
            char e[100] = "Error opening file: ";
            throw runtime_error(strcat(e, c_string_fileName));
        }
    }catch(char** e){
        cout << e << "\n";
        exit(1);
    }

    //read the input file and load the processes
    RoundRobin::read_processes();

    //initialize the clock to 0
    this->clock = 0;
    this->interval = 0;

    //add processes with a non-zero arrival time to list of unarrived_processes
    for(Process p : allProcesses){
        if(p.arrival_time > this->clock){
            unarrived_processes.push_back(p);
        }
    }

    //initialize the queue table
    RoundRobin::initQTable();

    RoundRobin::update_QTable();

    cout<< this->block_duration<<" "<< this->time_slice<<endl;
    RoundRobin::perform_processes();
}

/**
 * read_processes   -   reads from the input file and loads all of the processes
 */
void RoundRobin::read_processes() {
    const char* c_string_fileName = this->file_name.c_str();
    //Start reading the Input File
    //Make sure the file is open
    if(!inFile.is_open()){
        char e[100] = "Error opening file: ";
        throw runtime_error (strcat(e, c_string_fileName));
    }else{
        this->numTotalProcesses = 0;
        this->maxPriority = 0;
        //initialize variables to be used to create each Process
        string p_name;
        uint32_t priority;
        uint32_t arrival_time;
        uint32_t total_time;
        uint32_t block_interval;

        //read in the rest of the processes
        for(string line; getline(inFile, line); ){
            istringstream vals (line);

            //reset the variables with invalid values to allow for easier de-bugging
            p_name = "Wrong";
            priority = -1;
            arrival_time = -1;
            total_time = -1;
            block_interval = -1;

            //load the name of the process
            vals >> p_name;

            //load the "priority" value of the process
            vals>> priority;

            //load the "arrival_time" value of the process
            vals >> arrival_time;
            //load the "total_time" value of the process
            vals>> total_time;
            //load the "block-interval" value of the process
            vals>>block_interval;

            Process p = Process(p_name, priority, arrival_time, total_time, block_interval);

            //update the max priority if necessary
            if(p.priority > this->maxPriority){
                this->maxPriority = p.priority;
            }
            //insert the process into the vector
            allProcesses.push_back(p);
        }
        //track the total number of processes loaded to later check for completion
        numTotalProcesses = allProcesses.size();
    }
}


/**
 * perfom_processes -   this is the main function of the scheduler, it executes each of the ready
 *                      processes until they have all terminated
 */
void RoundRobin::perform_processes(){
    //Loop until completion
    while(this->finished_processes.size()!= this->numTotalProcesses){
        cout<<" "<<clock<<"\t";

        //check if IDLE -> checking sum of blocked + unarrived + finished == total is faster than checking the whole Queue Table
        if((this->blockedProcesses.size() + this->unarrived_processes.size() + this->finished_processes.size()) == this->numTotalProcesses) {

            //yes idle, calculate time until not idle
            int idle_time = 0;
            this->interval = 1;
            while((this->blockedProcesses.size()
                   + this->unarrived_processes.size()
                   + this->finished_processes.size())
                  == this->numTotalProcesses){
                update_QTable();
                idle_time += 1;
            }

            this->interval = idle_time;
            cout<<"(IDLE)\t"<<this->interval<<"\t"<<"I"<<endl;

            this->clock += this->interval;
        }

            //NOT idle
        else{
            bool block_after_processing = false;

            //get next highest-priority process from QTable
            int pri = 1;
            while(qTable.at(qTable.size() - pri).empty()){
                pri++;
            }

            Process p = qTable.at(qTable.size() - pri).front();

            //remove selected process from qTable
            auto it = qTable.at(qTable.size() - pri).begin();
            qTable.at(qTable.size() - pri).erase(it);

            //check if first time starting process
            if(p.start_time < 0){
                p.start_time = clock;
            }

            cout<<p.p_name<<"\t";

            /*to determine end condition of process execution,
             * load all of the factors that COULD be the reason, sort them,
             * and which ever happens first (has the lowest value),
             * is the reason the process was stopped. */
            vector<int>p_times;
            p_times.push_back(this->time_slice);
            p_times.push_back(p.block_interval);
            p_times.push_back(p.time_until_next_block);
            p_times.push_back(p.time_until_finished);

            sort(p_times.begin(), p_times.end());

            /*
             * in terms of priority of end conditions,
             * terminated > block > slice
             */
            if(p_times.at(0) == p.time_until_finished){
                //the process has terminated
                this->interval = p.time_until_finished;

                cout<<this->interval<<"\tT";
                p.is_finished = true;
                p.finish_time = clock + p.time_until_finished;
                p.turnaround_time = p.finish_time - p.arrival_time;
                p.time_until_finished = 0;
                this->finished_processes.push_back(p);
            }
            else if(p_times.at(0) == p.time_until_next_block){
                //the process has blocked
                this->interval = p.time_until_next_block;

                cout<<this->interval<<"\tB";

                p.time_until_finished -= this->interval;
                p.time_until_next_block -= this->interval;
                p.block_time_remaining = this->block_duration;

                block_after_processing = true;
            }
            else if(p_times.at(0) == this->time_slice && p_times.at(0) != p.time_until_next_block){
                //the time slice ended
                this->interval = this->time_slice;

                cout<<this->interval<<"\tS";

                /* update time until next block:
                 * since we already checked for p.time_until_next_block,
                 * we can assume time_slice > p.time_until_next_block */
                p.time_until_finished -= this->interval;
                p.time_until_next_block -= this->interval;

                if(p.time_until_next_block == 0){
                    //if it should now block
                    p.time_until_next_block = -1;

                    //remove from qTable and add to blocked
                    p.time_until_next_block = p.block_interval;
                    block_after_processing = true;
                } else {
                    //re-add to qTable
                    qTable.at(p.priority).push_back(p);
                }
            }

            //update Queue table
            update_QTable();

            /* add to blocked processes AFTER updated Queue table so this process's block_time_remaining does not
             * start being subtracted until it finishes executing */
            if(block_after_processing){
                //add to vector of blocked processes
                this->blockedProcesses.push_back(p);
            }

            cout<<endl;

            //update clock
            this->clock += this->interval;
        }
    }

    //calculate and output average turnaround time
    double avg_turnaround_time = calc_avg_turnaround();
    cout<<" "<<this->clock<<"\t(END)\t"<<avg_turnaround_time<<endl;
}

/**
 * update_QTable    - method used to update the Queue Table (qTable) as processes arrive, block, finish, and become unblocked
 */
void RoundRobin::update_QTable() {
    //update the list of blocked processes and unarrived-processes simultaneously to keep the Queue Table accurate
    for(int i = 0; i < this->interval; i++){

        /*update the blocked_time_remaining of each blocked process,
         * if updated to == 0, add to QTable and remove from list of blocked */
        for(auto it = this->blockedProcesses.begin(); it != this->blockedProcesses.end(); ){
            Process& test_proc_unblocked = *it;

            (*it).block_time_remaining -= 1;

            if((*it).block_time_remaining <= 0){
                //reset time until next block
                (*it).time_until_next_block = (*it).block_interval;
                (*it).block_time_remaining = 0;

                //add to QTable at appropriate priority level
                this->qTable.at((*it).priority).push_back((*it));
                //remove from list of blocked processes
                it = this->blockedProcesses.erase(it);
            } else {
                ++it;
            }
        }

        //purge list of unarrived processes of any newly-arrived
        for(auto it = this->unarrived_processes.begin(); it != this->unarrived_processes.end(); ){
            (*it).time_until_arrival -= 1;

            if((*it).time_until_arrival <= 0){
                //push to Queue Table at appropriate priority level
                this->qTable.at((*it).priority).push_back((*it));
                //remove from list of unarrived processes
                it = this->unarrived_processes.erase(it);
            } else {
                ++it;
            }
        }
    }
}

/**
 * initQTable   -   initializes the Queue Table to be a vector<vector<Process>>
 *                  to keep track of each priority level's queue
 */
void RoundRobin::initQTable() {

    //init the vector<vector<Process>> to have 9 priority levels
    //  (assumption of 9 being highest level of priority based on assignment page "Input Format"
    this->qTable = vector<vector<Process>>();
    for(int i = 0; i <= 9; i++){
        qTable.push_back(vector<Process>());
    }
    //add processes with arrival time of 0
    for(Process p : this->allProcesses){
        if(p.arrival_time == 0){
            this->qTable.at(p.priority).push_back(p);
        }
    }
}

/**
 * calc_avg_turnaround - calculates the average turnaround time for all of the Processes
 *
 * @return      the average turnaround time
 */
double RoundRobin::calc_avg_turnaround() {
    double totalTurnaround = 0;
    double numProc = this->numTotalProcesses;
    for(Process p : this->finished_processes){
        totalTurnaround += p.turnaround_time;
    }

    return totalTurnaround / numProc;
}

/**
 * printQTable  -   a helper method used mostly in debugging,
 *                  prints the contents of the Queue Table
 */
void RoundRobin::printQTable() {
    cout<<"\nprintQTable"<<endl;
    for(int i = 0; i < qTable.size(); i++){
        cout<<"["<<i<<"] - ";
        //make copy of queue so we dont have to worry about actually removing elements
        vector<Process> queue_cpy = qTable.at(i);
        if(!queue_cpy.empty()){
            cout<<endl;
        }
        while(!queue_cpy.empty()){
            Process cpy_p = queue_cpy.front();
            cout<<"\t";
            cpy_p.print();
            cout<<endl;
            auto it = queue_cpy.begin();
            queue_cpy.erase(it);
        }
        cout<<endl;
    }
}

/**
 * print_all_processes  - a helper method used mostly in debugging,
 *                          prints the entire vector of processes
 */
void RoundRobin::print_all_processes() {
    for(Process pro : this->allProcesses){
        cout<< pro.p_name << ' '
            << pro.priority << ' '
            << pro.arrival_time << ' '
            << pro.total_time << ' '
            << pro.block_interval<<endl;
    }
    cout<<endl;
}

/**
 * print_all_processes  - a helper method used mostly in debugging,
 *                          prints the entire vector of processes
 */
void RoundRobin::print_all_blocked() {
    cout<<"All blocked: ";
    for(Process pro : this->blockedProcesses){
        pro.print();
        cout<<"\n\t\t - ";
    }
    cout<<endl;
}

/**
 * print_all_processes  - a helper method used mostly in debugging.
 *                          prints the entire vector of processes
 */
void RoundRobin::print_all_unarrived() {
    cout<<"All unarrived: ";
    for(Process pro : this->unarrived_processes){
        pro.print();
        cout<<" ";
    }
    cout<<endl;
}

/**
 * print_all_processes  - a helper method used mostly in debugging,
 *                          prints the entire vector of processes
 */
void RoundRobin::print_all_finished() {
    cout<<"All finished: ";
    for(Process pro : this->finished_processes){
        pro.print();
        cout<<"\n\t";
    }
    cout<<endl;
}


