#include <vector>
#include "task.hpp"

class SJF
{
public:
    void schedule(std::vector<Task> &processes, SchedulerStats &stats);
    void printResults(std::vector<Task> &processes, SchedulerStats &stats);
};