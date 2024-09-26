#include "pthread-internal.h"

int pthread_cond_broadcast(pthread_cond_t *cond) {
    emfiberthreads_wake(&cond->waiters);
    return 0;
}
