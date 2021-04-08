# SchedulingSimulation

## Overview
This is an implementation of a preemptive Round-Robin scheduling algorithm with priorities, run on a specified mix of processes. 
The input to the program will specify scheduler parameters such as the time slice and the length of time that a process is unavailable to run when it blocks. Input will also include a list of processes to be scheduled.

## Command Line
The program accepts three arguments on the command line:

1. The input file name
2. block_duration: the decimal integer time length that a process is unavailable to run after it blocks
3. time_slice: the decimal integer length of the time slice for the Round-Robin scheduler 

## Input format
The input file, specified as the first command line argument, contains the list of processes to schedule.

All numeric values in the input file are decimal integers. The time unit doesn't matter; you can think of it as milliseconds.

This input file will contain 1 line per process. Lines will be sorted in increasing order of arrival time in the system. Each line will have the following format:
```
name priority arrival_time total_time block_interval
```

- `name` is a sequence of non-blank characters representing the name of the process
- `priority` is the priority level for the process, from 0 to 9. A higher number indicates a process should take precedence over lower-numbered priority processes (these are absolute priorities).
- `arrival_time` is the time at which the process arrives in the system
- `total_time` is the total amount of CPU time which will be used by the process
- `block_interval` is the interval at which the process will block for I/O. When a process blocks, it is unavailable to run for the time specified by the command line argument block_duration.

## Output Format
All output should be written to standard output. A set of lines should be written consisting of:

- A single line containing the `block_duration` and `time_slice` as specified on the command line. Values should be separated by spaces.
- One line for each interval during which a process is running or the system is idle. The line should consist of a single space, followed by the current simulation time (starting at 0), followed by the process `name` (or "(IDLE)" if no process is running), the length of the interval, and a status code indicating why the interval ended: "B" for blocked, "S" for time slice ended, "T" if the process terminated, or "I" for an idle interval. The fields should be separated by the tab character, '\t'.
- After all jobs have terminated, write a line consisting of a single space, the simulation time at which the last job terminated, a tab character, the string "(END)", another tab character, and the average turnaround time of all processes (floating point value).

## Sample Input and Output
Sample input and output files are supplied in **inputData**. The two .txt files are input (use only one input file for any execution of the program). Corresponding sample output files are given for different combinations of `block_duration` and `time_slice`.

Here is a sample input file, *program1_joblist1.txt*:

```
A 0 0 100 25
B 5 1 50 20
C 2 2 90 45
```

Here is a sample output from running this sample with `block_duration` = 20 and `time_slice` = 10 (program1_joblist1_20_10.out):

```
20 10
 0	A	10	S
 10	B	10	S
 20	B	10	B
 30	C	10	S
 40	C	10	S
 50	B	10	S
 60	B	10	B
 70	C	10	S
 80	C	10	S
 90	B	10	T
 100	C	5	B
 105	A	10	S
 115	A	5	B
 120	(IDLE)	5	I
 125	C	10	S
 135	C	10	S
 145	C	10	S
 155	C	10	S
 165	C	5	T
 170	A	10	S
 180	A	10	S
 190	A	5	B
 195	(IDLE)	20	I
 215	A	10	S
 225	A	10	S
 235	A	5	B
 240	(IDLE)	20	I
 260	A	10	S
 270	A	10	S
 280	A	5	T
 285	(END)	184
```

Here's a sample output from the same process list, with a `block_duration` of 50 and a `time_slice` of 10 (program1_joblist1_50_10.out):

```
50 10
 0	A	10	S
 10	B	10	S
 20	B	10	B
 30	C	10	S
 40	C	10	S
 50	C	10	S
 60	C	10	S
 70	C	5	B
 75	A	10	S
 85	B	10	S
 95	B	10	B
 105	A	5	B
 110	(IDLE)	15	I
 125	C	10	S
 135	C	10	S
 145	C	10	S
 155	B	10	T
 165	C	10	S
 175	C	5	T
 180	A	10	S
 190	A	10	S
 200	A	5	B
 205	(IDLE)	50	I
 255	A	10	S
 265	A	10	S
 275	A	5	B
 280	(IDLE)	50	I
 330	A	10	S
 340	A	10	S
 350	A	5	T
 355	(END)	232.333
```