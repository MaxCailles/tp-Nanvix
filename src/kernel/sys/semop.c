#include <nanvix/syscall.h>
#include <errno.h>
#include <sys/sem.h>

PUBLIC int sys_semop(int semid, int op){
     semid ++;
    op++;
    return 0;
}
