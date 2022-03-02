/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEM_H_
#define SEM_H_

#include <nanvix/pm.h>

/* semaphore  definition*/
typedef struct
{
	int key;					   // Key linked
	int value;					   // value to increment or decrement
	struct process *waiting_queue; // waiting list of blocked process by this semaphore
	int valide;
} semaphore;

/**
 * @brief Comand values for semaphores.
 */
/**@{*/
#define GETVAL 0   /**< Returns the value of a semaphore. */
#define SETVAL 1   /**< Sets the value of a semaphore.    */
#define IPC_RMID 3 /**< Destroys a semaphore.            */
/**@}*/

/* Forward definitions. */
extern int semget(unsigned);
extern int semctl(int, int, int);
extern int semop(int, int);

#define NB_SEM_MAX 2 * PROC_MAX // At least 2 semaphores per proc

EXTERN void initSempaphores();
EXTERN int create(int key);
EXTERN int up(int idSem);
EXTERN int down(int idSem);
EXTERN int destroy(int idSem);
EXTERN void setval(int idSem, int val);
EXTERN int getval(int idSem);
EXTERN int isvalid(int semid);

#endif /* SEM_H_ */
