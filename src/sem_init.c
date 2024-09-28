#include "pthread-internal.h"
#include "../include/semaphore.h"

int sem_init(sem_t *sem, int shared, unsigned int value) {
    (void) shared;
    sem->waiters = NULL;
    sem->value = value;
    return 0;
}
