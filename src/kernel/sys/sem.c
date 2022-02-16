/* Implémentation de l'abstraction sémaphore sem.h */

#include <nanvix/pm.h>
#include <nanvix/syscall.h>

/* semaphore  definition*/
typedef struct
{
    int key;                  // Key linked
    int value;                     // value to increment or decrement
    struct process *waiting_queue; // waiting list of blocked process by this semaphore
    int valide;
} semaphore;

static semaphore semaphores[NB_SEM_MAX]; // Array of all semaphores in use.
static int nbSem = 0;

void initSempaphores()
{
    for (int i = 0; i < NB_SEM_MAX; i++)
    {
        semaphores[i].valide = 0;
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
    semaphore newSemaphore = {key, 0, NULL, 1};
    semaphores[nbSem] = newSemaphore;
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
            struct process p = *(semaphores[idSem].waiting_queue);
            semaphores[idSem].waiting_queue = p.nextInSem;
        }
    }
    else
    {
        semaphores[idSem].value++;
    }
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
        struct process *old_head = semaphores[idSem].waiting_queue;
        semaphores[idSem].waiting_queue = curr_proc;
        curr_proc->nextInSem = old_head;
    }
    return 0;
}

int destroy(int idSem)
{
    semaphores[idSem].valide = 0;
    return 0;
}