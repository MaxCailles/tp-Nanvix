#include <nanvix/syscall.h>
#include <nanvix/pm.h>
#include <sys/sem.h>

PUBLIC int sys_semctl(int semid, int cmd, int val){
   
    /*  Semid verifications */

    if((semid < 0) || (semid  >= NB_SEM_MAX)){
        return -1;
    }
    
    if (!isvalid(semid)){
        return -1;
    }
   
    /* cmd verifications */
    
    switch(cmd){
        case GETVAL : 
            getval(semid);
        break;

        case SETVAL :  
            if (val<0){
                return -1;
            }
            setval(semid,val);
        break;

        case IPC_RMID: destroy(semid);
        break;

        default : return -1;
    }




    return 0;
}