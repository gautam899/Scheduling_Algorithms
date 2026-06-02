#include <iostream>
#include <algorithm>
#include "sjf.hpp"

void SJF::schedule(std::vector<Task> &processes, SchedulerStats &stats)
{
    int num_process = processes.size();

    std::sort(processes.begin(), processes.end(), [](Task &a, Task &b)
              { if(a.cpu_burst != b.cpu_burst) {
                    return a.cpu_burst < b.cpu_burst;
                }
                return a.arrival_time < b.arrival_time; });

    for (size_t i = 0; i < (size_t)num_process; i++)
    {
        processes[i].stats.start_time = (i == 0) ? processes[i].arrival_time : std::max(processes[i - 1].stats.completion_time, processes[i].arrival_time);
        processes[i].stats.completion_time = processes[i].stats.start_time + processes[i].cpu_burst;
        processes[i].stats.response_time = processes[i].stats.start_time - processes[i].arrival_time;
        processes[i].stats.turnaround_time = processes[i].stats.completion_time - processes[i].arrival_time;
        processes[i].stats.waiting_time = processes[i].stats.turnaround_time - processes[i].arrival_time;

        stats.total_response_time += processes[i].stats.response_time;
        stats.total_turnaround_time += processes[i].stats.turnaround_time;
        stats.total_waiting_time += processes[i].stats.waiting_time;
        stats.total_idle_time += (i == 0) ? processes[i].arrival_time : (processes[i].stats.start_time - processes[i - 1].stats.completion_time);
    }

    stats.av_responseTime = (float)stats.total_response_time / num_process;
    stats.av_turnaroundTime = (float)stats.total_turnaround_time / num_process;
    stats.av_waitingTime = (float)stats.total_waiting_time / num_process;
    stats.cpu_utilization = (float)(processes[num_process - 1].stats.completion_time - stats.total_idle_time) / processes[num_process - 1].stats.completion_time * 100;
    stats.throughput = (float)num_process / (processes[num_process - 1].stats.completion_time - processes[0].arrival_time);
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
        processes[i].pid += 1;
    }

    SJF shorted_job_first;
    shorted_job_first.schedule(processes, stats);
    shorted_job_first.printResults(processes, stats);

    return 0;
}