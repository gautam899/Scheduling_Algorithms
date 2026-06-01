#include <vector>
#include "task.hpp"

class FCFS
{
public:
    void schedule(std::vector<Task> &process, SchedulerStats &stats);
    void printResults(std::vector<Task> &process, SchedulerStats &stats);
};