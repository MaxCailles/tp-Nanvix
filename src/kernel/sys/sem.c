/* Implémentation de l'abstraction sémaphore sem.h */

#include <nanvix/pm.h>
#include <nanvix/klib.h>
#include <nanvix/syscall.h>


/* semaphore  definition*/
typedef struct
{
    int key;                  // Key linked
    int value;                     // value to increment or decrement
    struct process **waiting_queue; // waiting list of blocked process by this semaphore
    int valide;
} semaphore;

static semaphore semaphores[NB_SEM_MAX]; // Array of all semaphores in use.
static int nbSem = 0;

void initSempaphores()
{
    kprintf("Init Semaphores\n");
    for (int i = 0; i < NB_SEM_MAX; i++)
    {
        semaphores[i].key = 0;
        semaphores[i].valide = 0;
        semaphores[i].value = 0;
        semaphores[i].waiting_queue = NULL;
    }
}

int create(int key)
{
    /* check if the semaphore already exists */
    for (int i = 0; i < NB_SEM_MAX; i++)
    {
        if (semaphores[i].key == key)
        {
            if (semaphores[i].valide == 1)
            {
                return i;
            }
        }
    }

    /* Check if there is enough place */
    if (nbSem >= NB_SEM_MAX)
    {
        return -1;
    }

    /* create a new semaphore and save it in the array*/

    semaphores[nbSem].key = key;
    semaphores[nbSem].valide = 1;
    semaphores[nbSem].value = 0;
    semaphores[nbSem].waiting_queue = NULL;

    /* return the indice in the table */ 
    int ret = nbSem;
    nbSem++;
    return ret;
}

int getval(int idSem)
{
    return semaphores[idSem].value;
}

void setval(int idSem, int val)
{
    semaphores[idSem].value = val;
}

int isvalid(int semid)
{
    return semaphores[semid].valide;
}

int up(int idSem)
{
    if (semaphores[idSem].value == 0)
    {
        if (semaphores[idSem].waiting_queue != NULL)
        {
            wakeup(semaphores[idSem].waiting_queue);
        }
        
    }
    semaphores[idSem].value++; 

    return 0;
}

int down(int idSem)
{
    if (semaphores[idSem].value > 0)
    {
        semaphores[idSem].value--;
    }
    else
    {
        while(semaphores[idSem].value < 0){
            sleep(semaphores[idSem].waiting_queue,curr_proc->priority);
        }
        
    }
    return 0;

}

int destroy(int idSem)
{
    semaphores[idSem].valide = 0;
    return 0;
}