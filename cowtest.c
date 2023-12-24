#include "types.h"
#include "user.h"

int a = 1;

void test()
{
    printf(1, "------------------Test------------------\n");
    if (fork() == 0){
        printf(1, "C\ta = %d / free pages before changing a: %d\n", a,getNumFreePages());
        printf(1, "\n");
        sleep(10);
        a = 2;
        printf(1, "C\ta = %d / free pages after changing a: %d\n", a,getNumFreePages());
        exit();
    }
    else{
        printf(1, "P\ta = %d / free pages before changing: %d\n", a, getNumFreePages());
        sleep(15);
        printf(1, "P\ta = %d / free pages after changing a: %d\n",a, getNumFreePages());
        wait();
    }
    printf(1, "\nP\ta = %d / free pages after exiting C: %d\n",a, getNumFreePages());
}

int main(){
    printf(1, "\nfree pages before fork: %d\n", getNumFreePages());
    test();
    exit();
}
