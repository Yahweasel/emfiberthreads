#include "pthread-internal.h"

int pthread_cond_signal(pthread_cond_t *cond) {
    return emfiberthreads_wake_one(&cond->waiters);
}
