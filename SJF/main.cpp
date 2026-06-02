#include <iostream>
#include <algorithm>
#include <limits.h>
#include "sjf.hpp"

void SJF::schedule(std::vector<Task> &processes, SchedulerStats &stats)
{
    int num_process = processes.size();
    std::vector<int> isComplete(num_process, 0);
    // SJF does not know about the next processes so we cannot sort them before hand.
    int time = 0;
    int completed = 0; // Number of processes completed
    int prev_completion_time = 0;

    while (completed != num_process)
    {
        int min_burst = INT_MAX;
        int curr_process = -1; // Process to execute

        // Find the next minimum CPU burst
        for (size_t i = 0; i < num_process; i++)
        {
            if (processes[i].arrival_time <= time && isComplete[i] == 0)
            {
                if (processes[i].cpu_burst < min_burst)
                {
                    min_burst = processes[i].cpu_burst;
                    curr_process = i;
                }

                // If the cpu_burst time is the same for two processes, FCFS should be used as a tie breaker.
                if (processes[i].cpu_burst == min_burst && processes[i].arrival_time < processes[curr_process].arrival_time)
                {
                    min_burst = processes[i].cpu_burst;
                    curr_process = i;
                }
            }
        }

        if (curr_process != -1)
        {
            std::cout << "Executing Process: " << curr_process << std::endl;
            processes[curr_process].stats.start_time = time;
            processes[curr_process].stats.completion_time = processes[curr_process].stats.start_time + processes[curr_process].cpu_burst;
            processes[curr_process].stats.response_time = processes[curr_process].stats.start_time - processes[curr_process].arrival_time;
            processes[curr_process].stats.turnaround_time = processes[curr_process].stats.completion_time - processes[curr_process].arrival_time;
            processes[curr_process].stats.waiting_time = processes[curr_process].stats.turnaround_time - processes[curr_process].cpu_burst;

            stats.total_response_time += processes[curr_process].stats.response_time;
            stats.total_turnaround_time += processes[curr_process].stats.turnaround_time;
            stats.total_waiting_time += processes[curr_process].stats.waiting_time;
            stats.total_idle_time += processes[curr_process].stats.start_time - prev_completion_time;

            isComplete[curr_process] = 1;
            time = processes[curr_process].stats.completion_time;
            prev_completion_time = time;
            completed++;
        }
        else
        {
            time++;
        }
    }

    int max_completion_time = -1;
    int min_arrival_time = INT_MAX;
    for (size_t i = 0; i < num_process; i++)
    {
        max_completion_time = std::max(max_completion_time, processes[i].stats.completion_time);
        min_arrival_time = std::min(min_arrival_time, processes[i].arrival_time);
    }

    stats.av_responseTime = (float)stats.total_response_time / num_process;
    stats.av_turnaroundTime = (float)stats.total_turnaround_time / num_process;
    stats.av_waitingTime = (float)stats.total_waiting_time / num_process;
    stats.cpu_utilization = (float)(max_completion_time - stats.total_idle_time) / max_completion_time * 100;
    stats.throughput = (float)num_process / (max_completion_time - min_arrival_time);
}

void SJF::printResults(std::vector<Task> &processes, SchedulerStats &stats)
{
    int num_process = processes.size();
    // Sort as per the process Id
    std::sort(processes.begin(), processes.end(), [](Task &a, Task &b)
              { return a.pid < b.pid; });

    // pid, Arrival Time, CPU burst time, Start Time, Completion Time, Turn Around Time, Waiting Time, Response Time
    std::cout << "#Pid\t" << "A_Time\t" << "B_Time\t" << "S_Time\t" << "C_Time\t" << "T_A_T\t" << "W_Time\t" << "R_Time\t" << std::endl;
    for (int i = 0; i < num_process; i++)
    {
        std::cout << processes[i].pid << "\t" << processes[i].arrival_time << "\t" << processes[i].cpu_burst << "\t" << processes[i].stats.start_time << "\t" << processes[i].stats.completion_time << "\t" << processes[i].stats.turnaround_time << "\t" << processes[i].stats.waiting_time << "\t" << processes[i].stats.response_time << std::endl;
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
    std::cout << "Enter the number of process the user wish to execute -> ";
    std::cin >> num_process;
    if (num_process <= 0)
    {
        fprintf(stderr, "Number of Process Must be greater than zero\n");
        return 1;
    }

    std::vector<Task> processes(num_process);
    SchedulerStats stats;

    for (size_t i = 0; i < num_process; i++)
    {
        std::cout << "Enter the Arrival Time of the Process " << i + 1 << " : -> ";
        std::cin >> processes[i].arrival_time;
        std::cout << "Enter the CPU burst time for the Process " << i + 1 << " : -> ";
        std::cin >> processes[i].cpu_burst;
        processes[i].pid = i + 1;
    }

    SJF shorted_job_first;
    shorted_job_first.schedule(processes, stats);
    shorted_job_first.printResults(processes, stats);

    return 0;
}