/* Implémentation de l'abstraction sémaphore sem.h */

#include <nanvix/pm.h>
#include <nanvix/klib.h>
#include <nanvix/syscall.h>


semaphore semaphores[NB_SEM_MAX]; // Array of all semaphores in use.
int nbSem = 0;
int indic_min = 0;

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

    /* Not Found so we ha ve to create a new one*/

    /* Check if there is enough place */
    if (nbSem >= NB_SEM_MAX)
    {
        return -1;
    }

    /* create a new semaphore and save it in the array*/

    semaphores[indic_min].key = key;
    semaphores[indic_min].valide = 1;
    semaphores[indic_min].value = 0;
    semaphores[indic_min].waiting_queue = NULL;

    /* return the indice in the table */ 
    int ret = indic_min;

    /* compute the new free place in the array */
    int i = 0;
    int found = 0;
    while((i<NB_SEM_MAX) && (found==0) )
    {   if (semaphores[i].valide == 0){
        indic_min = i;
        found=1;
        }
        i++;
    }

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
    disable_interrupts();
    semaphores[idSem].value++;

    if (semaphores[idSem].value <= 0)
    {
        wakeuponeprocess(&(semaphores[idSem].waiting_queue));
    }
    enable_interrupts();
    return 0;
}

int down(int idSem)
{ 
    disable_interrupts();
    semaphores[idSem].value--;
    if (semaphores[idSem].value < 0)
    {
        sleep(&(semaphores[idSem].waiting_queue),curr_proc->priority);
        
    }
    enable_interrupts();
    return 0;

}

int destroy(int idSem)
{
    semaphores[idSem].valide = 0;
    if(idSem < indic_min){
        indic_min = idSem;
    }
    nbSem--;
    return 0;
}