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

#ifndef EMFIBERTHREADS_PTHREAD_H
#define EMFIBERTHREADS_PTHREAD_H 1

#include <emscripten/fiber.h>

/* Sufficient pthread-like API on top of Emscripten fibers */

struct emfiber_pthread_t;

/* These __DEFINED_ macros are to override musl. */
#define __DEFINED_pthread_t 1

/**
 * emfiberthread pthreads are allocated structures.
 */
typedef struct emfiber_pthread_t *pthread_t;

/**
 * To rotate fibers, we keep them on a list.
 */
struct emfiber_list_t {
    pthread_t prev, next;
};

#define __DEFINED_pthread_mutex_t 1
typedef struct pthread_mutex_t {
    /**
     * Threads currently waiting for the lock. Linear linked list.
     */
    pthread_t waiters;

    /**
     * Thread currently holding the lock, or NULL for none. Not a list head.
     */
    pthread_t holder;
} pthread_mutex_t;

#define __DEFINED_pthread_once_t 1
typedef struct pthread_once_t {
    /**
     * Lock for this once structure.
     */
    pthread_mutex_t lock;

    /**
     * Set to 1 when this once has been done.
     */
    unsigned char done;
} pthread_once_t;

#define __DEFINED_pthread_cond_t 1
typedef struct pthread_cond_t {
    /**
     * Threads waiting on this condition, linear linked list.
     */
    pthread_t waiters;
} pthread_cond_t;

#define __DEFINED_pthread_attr_t 1
typedef int pthread_attr_t;
#define __DEFINED_pthread_mutexattr_t 1
typedef int pthread_mutexattr_t;
#define __DEFINED_pthread_condattr_t 1
typedef int pthread_condattr_t;

#define PTHREAD_MUTEX_INITIALIZER { NULL, NULL }
#define PTHREAD_ONCE_INIT { PTHREAD_MUTEX_INITIALIZER, 0 }
#define PTHREAD_COND_INITIALIZER { NULL }

#include <time.h>

/* We rename functions to avoid clashes with stub libraries. */
#define pthread_create emfiberthreads_pthread_create
int pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
#define pthread_join emfiberthreads_pthread_join
int pthread_join(pthread_t, void **);
#define pthread_exit emfiberthreads_pthread_exit
void pthread_exit(void *);
#define pthread_self emfiberthreads_pthread_self
pthread_t pthread_self(void);
#define pthread_yield emfiberthreads_pthread_yield
int pthread_yield(void);

#define pthread_mutex_init emfiberthreads_pthread_mutex_init
int pthread_mutex_init(pthread_mutex_t *, const pthread_mutexattr_t *);
#define pthread_mutex_destroy emfiberthreads_pthread_mutex_destroy
static inline int pthread_mutex_destroy(pthread_mutex_t *mutex) { (void) mutex; return 0; }
#define pthread_mutex_lock emfiberthreads_pthread_mutex_lock
int pthread_mutex_lock(pthread_mutex_t *);
#define pthread_mutex_trylock emfiberthreads_pthread_mutex_trylock
int pthread_mutex_trylock(pthread_mutex_t *);
#define pthread_mutex_unlock emfiberthreads_pthread_mutex_unlock
int pthread_mutex_unlock(pthread_mutex_t *);

#define pthread_once emfiberthreads_pthread_once
int pthread_once(pthread_once_t *, void (*)(void));

#define pthread_cond_init emfiberthreads_pthread_cond_init
int pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
#define pthread_cond_destroy emfiberthreads_pthread_cond_destroy
static int pthread_cond_destroy(pthread_cond_t *cond) { (void) cond; return 0; }
#define pthread_cond_broadcast emfiberthreads_pthread_cond_broadcast
int pthread_cond_broadcast(pthread_cond_t *);
#define pthread_cond_signal emfiberthreads_pthread_cond_signal
int pthread_cond_signal(pthread_cond_t *);
#define pthread_cond_wait emfiberthreads_pthread_cond_wait
int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
#define pthread_cond_timedwait emfiberthreads_pthread_cond_timedwait
int pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);

#endif
