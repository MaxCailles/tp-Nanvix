/* Implémentation de l'abstraction sémaphore sem.h */

#include <sys/sem.h>
#include <nanvix/syscall.h>

/* semaphore  definition*/
typedef struct {
    unsigned key;  // Key linked 
    int value;     // value to increment or decrement
    struct process *wainting_queue;   // waiting list of blocked process by this semaphore
    struct semaphore *next;           // next semaphore in the linked chain of semaphores 

} semaphore;

semaphore *semaphores_head;  // Linked list of all semaphores in use.

PUBLIC int semget(unsigned key){
    return 0;
}



PUBLIC int semctl(int semid, int cmd, int val){
    return 0;
}



PUBLIC int semop(int semid, int op){
    return 0;
}