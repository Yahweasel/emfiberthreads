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

#include "pthread-internal.h"
#include "../include/semaphore.h"

int sem_wait(sem_t *sem) {
    EMFT_INIT();
    if (sem->value) {
        sem->value--;
        return 0;
    }
    emfiberthreads_self->list.prev->list.next = emfiberthreads_self->list.next;
    emfiberthreads_self->list.next->list.prev = emfiberthreads_self->list.prev;
    emfiberthreads_self->list.prev = NULL;
    emfiberthreads_self->list.next = sem->waiters;
    if (sem->waiters)
        sem->waiters->list.prev = emfiberthreads_self;
    sem->waiters = emfiberthreads_self;
    pthread_yield();
    return 0;
}
