#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int pid = getpid();
    int priority = atoi(argv[1]);
    priority = set_proc_priority(pid, priority);
    printf(1, "PID(%d) with priority (%d) is running...\n", pid, priority);
    while (1);
}
