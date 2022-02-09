/* Implémentation de l'abstraction sémaphore sem.h */


#include <sys/sem.h>
#include <nanvix/pm.h>
#include <nanvix/syscall.h>

/* semaphore  definition*/
typedef struct {
    unsigned key;  // Key linked 
    int value;     // value to increment or decrement
    struct process *waiting_queue;   // waiting list of blocked process by this semaphore
} semaphore;

semaphore semaphores[20];  // Array of all semaphores in use.
int nbSem=0;


int create(int n, unsigned key){
    /* check if the semaphore already exists */
    for(int i=0; i<20;i++){
        if (semaphores[i].key == key){
            return i;
        }
    }

    /* Check if there is enough place */
    if (nbSem>=20){  
        return -1;
    }

    /* create a new semaphore and save it in the array*/ 
    semaphore newSemaphore = {key,n,NULL}; 
    semaphores[nbSem] = newSemaphore;
    int ret = nbSem;
    nbSem++;
    return ret;
}

int up(int idSem,struct process p){

    if(semaphores[idSem].value == 0){
        if(semaphores[idSem].waiting_queue != NULL){
            p = *(semaphores[idSem].waiting_queue);
            semaphores[idSem].waiting_queue = p.next;
        }
    } else {
        semaphores[idSem].value++;
    }
    return 0;
}

/*
* 
* ATTENTION CETTE IMPLEMENTATION N'EST PAS FIFO MAIS LIFO !
* Il reste à créer une fonction de recupération de queue de liste chainée.
*
*/

int down(int idSem,struct process p){
     if(semaphores[idSem].value > 0){
         semaphores[idSem].value--;
    } else {
        struct process* old_head = semaphores[idSem].waiting_queue;
        semaphores[idSem].waiting_queue = &p;
        p.next = old_head;

    }
    return 0;
}

int destroy(int idSem){
    idSem++;
    return 0;
}