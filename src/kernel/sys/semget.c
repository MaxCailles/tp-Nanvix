#include <nanvix/syscall.h>
#include <sys/sem.h>
#include <errno.h>

PUBLIC int sys_semget(unsigned key){
	//int ret = create(1,key);
    key++;
    return 0;
}