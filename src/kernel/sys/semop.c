#include <nanvix/syscall.h>
#include <nanvix/pm.h>

PUBLIC int sys_semop(int semid, int op)
{
    /*  Semid verifications */
    if((semid < 0) || (semid  >= NB_SEM_MAX)){
        return -1;
    }
    
    if (!isvalid(semid)){
        return -1;
    }

    /* op verifications */
    switch (op)
    {
    case 1:
        up(semid);
      
        break;

    case -1:
        down(semid);
        break;

    default:
        return -1;
    }

    return 0;
}
