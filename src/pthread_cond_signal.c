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

int pthread_cond_signal(pthread_cond_t *cond) {
    if (cond->waiters) {
        pthread_t next;

        EMFT_INIT();

        next = cond->waiters->list.next;

        cond->waiters->list.prev = emfiberthreads_self;
        cond->waiters->list.next = emfiberthreads_self->list.next;
        emfiberthreads_self->list.next->list.prev = cond->waiters;
        emfiberthreads_self->list.next = cond->waiters;
        emfiberthreads_next = cond->waiters;
        cond->waiters = next;
    }

    return 0;
}
