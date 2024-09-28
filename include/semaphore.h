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

#define sem_init emfiberthreads_sem_init
int sem_init(sem_t *, int, unsigned int);
#define sem_wait emfiberthreads_sem_wait
int sem_wait(sem_t *);
#define sem_trywait emfiberthreads_sem_trywait
int sem_trywait(sem_t *);
#define sem_post emfiberthreads_sem_post
int sem_post(sem_t *);
#define sem_getvalue emfiberthreads_sem_getvalue
static inline int sem_getvalue(sem_t *sem, int *value) { *value = (int) sem->value; return 0; }
#define sem_destroy emfiberthreads_sem_destroy
static inline int sem_destroy(sem_t *sem) { (void) sem; return 0; }

#endif
