struct TaskStats
{
    int start_time;
    int completion_time;
    int response_time;
    int turnaround_time;
    int waiting_time;
};

struct Task
{
    int pid;
    int arrival_time;
    int cpu_burst;
    TaskStats stats;
};

struct SchedulerStats
{
    float av_turnaroundTime = 0.0;
    float throughput = 0.0;
    float av_responseTime = 0.0;
    float av_waitingTime = 0.0;

    int total_response_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_idle_time = 0;
    float cpu_utilization = 0.0;
};