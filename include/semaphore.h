/*
 * Copyright (C) 2024 Yahweasel and contributors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef EMFIBERTHREADS_SEMAPHORE_H
#define EMFIBERTHREADS_SEMAPHORE_H 1

#include "pthread.h"

/**
 * emfiberthreads semaphore.
 */
typedef struct emfiberthreads_sem_t {
    /**
     * Threads waiting on the semaphore.
     */
    pthread_t waiters;

    /**
     * Current value of the semaphore.
     */
    unsigned int value;
} sem_t;

int sem_init(sem_t *, int, unsigned int);
int sem_wait(sem_t *);
int sem_trywait(sem_t *);
int sem_post(sem_t *);
static inline int sem_getvalue(sem_t *sem, int *value) { *value = (int) sem->value; return 0; }
static inline int sem_destroy(sem_t *sem) { (void) sem; return 0; }

#endif
