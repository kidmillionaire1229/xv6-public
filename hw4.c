#include "types.h"
#include "stat.h"
#include "user.h"

int a = 1;

void cowtest()
{
    printf(1, "[ fork 수행 전 ]\n");
    printf(1,"free page 개수: %d\n",getNumFreePages());
    printf(1,"전역변수 1을 참조: a = %d\n",a);
    printf(1, "[ fork 수행 ]\n");    
    int pid = fork();
    
    //printf(1, "[ fork 수행 후 ]\n");    
    if(pid==0)
    {
        sleep(5); 
       // printf(1, "[ fork 수행 후 ]\n");    
        printf(1,"[Child Process : a = %d]\n",a);
        printf(1,"a의 값 수정 전 free page 개수: %d\n",getNumFreePages());
        //sleep(5); 

        a = 2;
        printf(1,"[ Child Process : a = %d]\n",a);
        printf(1,"a의 값 수정 후 free page 개수: %d\n",getNumFreePages());
        printf(1, "[ exit 수행] \n"); 
        exit();
    }
    
    wait();
    printf(1,"[ Parent Process : a = %d]\n",a);
    printf(1,"chile process exit, Parent Process wait 후의 free page 개수 : %d\n",getNumFreePages());
    return ;
}


int main(void)
{
    printf(1,"[테스트 시작]\n");
    cowtest();
    printf(1,"[테스트 완료]\n");

    exit();
}
