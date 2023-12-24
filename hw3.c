#include "types.h"
#include "user.h"

void set_get_priority_test()
{
    printf(1, "[get_proc_priority, set_proc_priority 테스트]\n");

    //기본 priority 5 테스트 
    printf(1, "pid 1 default priority : %d\n",get_proc_priority(1));    
    printf(1, "pid 2 default priority : %d\n\n",get_proc_priority(2));        

    //임의의 priority set, get 테스트 
    printf(1, "pid 0 result of set_proc_priority(0,4) : %d\n", set_proc_priority(0, 4)); 
    printf(1, "pid 1 result of set_proc_priority(1,3) : %d\n\n", set_proc_priority(1, 3)); 

    printf(1, "pid 0 result of get_proc_priority(0) : %d\n",get_proc_priority(0));    
    printf(1, "pid 1 result of get_proc_priority(1) : %d\n\n",get_proc_priority(1));    
    
    printf(1, "set/get priority test finished\n\n");
}

void fork_priority_test()
{
    printf(1, "[fork시, child process의 priority 테스트]\n");
    
    //부모 프로세스는 priority 4로 초기화 
    int parent_pid = getpid();
    printf(1, "parent pid result of set_proc_priority(%d,4) : %d\n", parent_pid, set_proc_priority(parent_pid, 4)); 

    int pid = fork();

    //자녀 프로세스의 priority를 출력 
    if (pid < 0)
        printf(1, "fork failed"); 
    else if (pid == 0)
        exit();
    else{
        printf(1, "child pid result of get_proc_priority(%d,4) : %d\n\n", pid, get_proc_priority(pid)); 
        wait();
        printf(1, "fork test finished\n\n");
    }
}

int infinite_loop_exec(char *priority)
{
    char *args[] = {"infinite_loop_example", priority, 0};
    
    int pid = fork();

    if (pid < 0)
        printf(1, "fork failed");
    else if (pid == 0)
        exec(args[0], args);
    else
        set_proc_priority(pid, atoi(priority));
    
    return pid;
}


void no_starvation_test()
{
    printf(1, "[Starvation 일어나지 않는지 테스트]\n");
    
    int parent_pid = getpid();
    printf(1, "parent pid result of set_proc_priority(%d,5) : %d\n\n", parent_pid, set_proc_priority(parent_pid, 5)); 
    
    //자녀 프로세스 각각 priority 3,4로 설정 
    int child1 = infinite_loop_exec("3"); 
    int child2 = infinite_loop_exec("4"); 
    
    sleep(10); 
    sleep(10);

    //부모 프로세스에서 starvation 없이 자녀프로세스 kill가능 
    kill(child1); 
    kill(child2);
    
    wait();
    wait();

    printf(1,"no starvation test finished\n"); 
}



int main(int argc, char *argv[])
{
    
    set_get_priority_test();
    fork_priority_test(); 
    no_starvation_test();     
    exit();
}
