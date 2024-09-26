#include "pthread-internal.h"

pthread_t pthread_self() {
    return emfiberthreads_self;
}
