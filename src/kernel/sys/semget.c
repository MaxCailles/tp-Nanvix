#include <nanvix/syscall.h>
#include <nanvix/pm.h>


PUBLIC int sys_semget(int key){
	
    /*  key verifications */
    if ((0 > key) || (key >= NB_SEM_MAX)){
        return -1;
    }

    return create(key);
}