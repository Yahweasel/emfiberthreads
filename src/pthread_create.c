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

#include <errno.h>
#include <stdlib.h>

#include "pthread-internal.h"

#define STACK_SIZE 65536
#define ASYNCIFY_STACK_SIZE 4096

struct FiberEntryArg {
    void *(*entry)(void *);
    void *arg;
};

static void fiberEntry(void *argVP) {
    struct FiberEntryArg *arg;
    void *(*entry)(void *);
    void *ret;

    arg = (struct FiberEntryArg *) argVP;
    entry = arg->entry;
    ret = arg->arg;
    free(arg);

    pthread_exit(entry(ret));
}

int pthread_create(
    pthread_t *thrPtr, const pthread_attr_t *attr,
    void *(*entry)(void *), void *arg
) {
    pthread_t thr;
    int ret;
    struct FiberEntryArg *feArg;

    (void) attr;

    if (!emfiberthreads_mainFiber) {
        void *asyncifyStack;
        pthread_t cleanupThr;

        /* No main fiber. Initialize it. */
        emfiberthreads_mainFiber = calloc(1, sizeof(struct emfiber_pthread_t));
        if (!emfiberthreads_mainFiber)
            return errno;
        emfiberthreads_mainFiber->list.next = emfiberthreads_mainFiber;
        emfiberthreads_mainFiber->list.prev = emfiberthreads_mainFiber;

        emfiberthreads_mainFiber->asyncifyStack = asyncifyStack =
            calloc(ASYNCIFY_STACK_SIZE, 1);
        if (!asyncifyStack)
            return errno;

        emscripten_fiber_init_from_current_context(
            &emfiberthreads_mainFiber->fiber,
            asyncifyStack, ASYNCIFY_STACK_SIZE
        );

        emfiberthreads_self = emfiberthreads_next = emfiberthreads_mainFiber;
    }

    /* Allocate a new fiber. */
    thr = calloc(1, sizeof(struct emfiber_pthread_t));
    if (!thr)
        return errno;
    *thrPtr = thr;

    /* And its stacks. */
    ret = posix_memalign(&thr->stack, 16, STACK_SIZE);
    if (ret != 0) {
        free(thr);
        return ret;
    }
    thr->asyncifyStack = calloc(ASYNCIFY_STACK_SIZE, 1);
    if (!thr->asyncifyStack) {
        ret = errno;
        free(thr->stack);
        free(thr);
        return ret;
    }
    feArg = calloc(1, sizeof(struct FiberEntryArg));
    if (!feArg) {
        ret = errno;
        free(thr->asyncifyStack);
        free(thr->stack);
        free(thr);
        return ret;
    }

    feArg->entry = entry;
    feArg->arg = arg;

    /* Create the fiber. */
    emscripten_fiber_init(
        &thr->fiber, fiberEntry, (void *) feArg,
        thr->stack, STACK_SIZE,
        thr->asyncifyStack, ASYNCIFY_STACK_SIZE
    ); 

    /* And add it to the runlist. */
    thr->list.next = emfiberthreads_self->list.next;
    thr->list.prev = emfiberthreads_self;
    emfiberthreads_self->list.next->list.prev = thr;
    emfiberthreads_self->list.next = thr;
    emfiberthreads_next = thr;

    return 0;
}
