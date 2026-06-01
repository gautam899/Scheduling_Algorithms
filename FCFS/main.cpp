#include <iostream>
#include <vector>
#include <algorithm>
#include "fcfs.hpp"

void FCFS::schedule(std::vector<Task> &process, SchedulerStats &stats)
{
    int num_process = process.size();
    // Sort the processes upon there arrival time
    std::sort(process.begin(), process.end(), [](Task &a, Task &b)
              { return a.arrival_time < b.arrival_time; });

    // Iterate through all the processes and decide their execution order.
    for (int i = 0; i < num_process; i++)
    {
        process[i].stats.start_time = (i == 0) ? process[i].arrival_time : std::max(process[i - 1].stats.completion_time, process[i].arrival_time);
        process[i].stats.completion_time = process[i].stats.start_time + process[i].cpu_burst;
        process[i].stats.turnaround_time = process[i].stats.completion_time - process[i].arrival_time;
        process[i].stats.waiting_time = process[i].stats.turnaround_time - process[i].cpu_burst;
        process[i].stats.response_time = process[i].stats.start_time - process[i].arrival_time;

        // Find the totals
        stats.total_response_time += process[i].stats.response_time;
        stats.total_waiting_time += process[i].stats.waiting_time;
        stats.total_turnaround_time += process[i].stats.turnaround_time;
        stats.total_idle_time += (i == 0) ? process[i].arrival_time : (process[i].stats.start_time - process[i - 1].stats.completion_time);
    }

    stats.av_responseTime = (float)stats.total_response_time / num_process;
    stats.av_turnaroundTime = (float)stats.total_turnaround_time / num_process;
    stats.av_waitingTime = (float)stats.total_waiting_time / num_process;
    stats.cpu_utilization = (float)(process[num_process - 1].stats.completion_time - stats.total_idle_time) / (float)process[num_process - 1].stats.completion_time * 100;
    stats.throughput = (float)num_process / (process[num_process - 1].stats.completion_time - process[0].arrival_time);
}

void FCFS::printResults(std::vector<Task> &process, SchedulerStats &stats)
{
    int num_process = process.size();
    // Sort as per the process Id
    std::sort(process.begin(), process.end(), [](Task &a, Task &b)
              { return a.pid < b.pid; });

    // pid, Arrival Time, CPU burst time, Start Time, Completion Time, Turn Around Time, Waiting Time, Response Time
    std::cout << "#Pid\t" << "A_Time\t" << "B_Time\t" << "S_Time\t" << "C_Time\t" << "T_A_T\t" << "W_Time\t" << "R_Time\t" << std::endl;
    for (int i = 0; i < num_process; i++)
    {
        std::cout << process[i].pid << "\t" << process[i].arrival_time << "\t" << process[i].cpu_burst << "\t" << process[i].stats.start_time << "\t" << process[i].stats.completion_time << "\t" << process[i].stats.turnaround_time << "\t" << process[i].stats.waiting_time << "\t" << process[i].stats.response_time << std::endl;
    }

    std::cout << "Average Response Time: " << stats.av_responseTime << std::endl;
    std::cout << "Average Turn Around Time: " << stats.av_turnaroundTime << std::endl;
    std::cout << "Average Waiting Time: " << stats.av_waitingTime << std::endl;
    std::cout << "Total CPU utilization: " << stats.cpu_utilization << "%" << std::endl;
    std::cout << "Throughput: " << stats.throughput << std::endl;
}

int main()
{
    int num_process = 0;
    std::cout << "Enter the number of Process -> ";
    std::cin >> num_process;
    if (num_process <= 0)
    {
        fprintf(stderr, "Number of Process Must be greater than zero\n");
        return 1;
    }
    std::vector<Task> process(num_process);
    SchedulerStats stats;
    FCFS scheduler;

    for (int i = 0; i < num_process; i++)
    {
        std::cout << "Enter the arrival time of the process " << i + 1 << " -> ";
        std::cin >> process[i].arrival_time;
        std::cout << "Enter the cpu burst time of the process in ms(millisecond) " << i + 1 << " -> ";
        std::cin >> process[i].cpu_burst;
        process[i].pid = i + 1;
    }

    scheduler.schedule(process, stats);
    scheduler.printResults(process, stats);

    return 0;
}

/*
    Example:

    Enter the number of Process -> 2
    Enter the arrival time of the process 1 -> 10
    Enter the cpu burst time of the process in ms(millisecond) 1 -> 30
    Enter the arrival time of the process 2 -> 20
    Enter the cpu burst time of the process in ms(millisecond) 2 -> 20
    #Pid    A_Time  B_Time  S_Time  C_Time  T_A_T   W_Time  R_Time
    1       10      30      10      40      30      0       0
    2       20      20      40      60      40      20      20
    Average Response Time: 10
    Average Turn Around Time: 35
    Average Waiting Time: 10
    Total CPU utilization: 83%
    Throughput: 0.04
*/