#include <nanvix/syscall.h>
#include <errno.h>
#include <sys/sem.h>

PUBLIC int sys_semctl(int semid, int cmd, int val){
    semid++;
    cmd++;
    val++;
    return 0;
}