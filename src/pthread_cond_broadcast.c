#include "pthread-internal.h"

int pthread_cond_broadcast(pthread_cond_t *cond) {
    return emfiberthreads_wake(&cond->waiters);
}
