CC=emcc
CFLAGS=-Oz -Wno-undefined-internal
AR=emar
RANLIB=emranlib
PREFIX=/usr
STACK_SIZE=65536
ASYNCIFY_STACK_SIZE=4096

OBJ=\
	obj/pthread_create.o \
	obj/pthread_join.o \
	obj/pthread_exit.o \
	obj/pthread_self.o \
	obj/pthread_yield.o \
	obj/pthread_mutex_init.o \
	obj/pthread_mutex_lock.o \
	obj/pthread_mutex_trylock.o \
	obj/pthread_mutex_unlock.o \
	obj/pthread_once.o \
	obj/pthread_cond_init.o \
	obj/pthread_cond_broadcast.o \
	obj/pthread_cond_signal.o \
	obj/pthread_cond_wait.o \
	obj/pthread_cond_timedwait.o \
	obj/emfiberthreads_sleep.o \
	obj/emfiberthreads_wake.o \
	obj/globals.o

all: libemfiberthreads.a

libemfiberthreads.a: $(OBJ)
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -DEMFIBERTHREADS_STACK_SIZE=$(STACK_SIZE) \
		-DEMFIBERTHREADS_ASYNCIFY_STACK_SIZE=$(ASYNCIFY_STACK_SIZE) \
		-c $< -o $@

install:
	mkdir -p $(PREFIX)/include/sys
	install -m 0644 include/pthread.h $(PREFIX)/include/pthread.h
	install -m 0644 include/signal.h $(PREFIX)/include/signal.h
	install -m 0644 include/mqueue.h $(PREFIX)/include/mqueue.h
	install -m 0644 include/sys/types.h $(PREFIX)/include/sys/types.h
	mkdir -p $(PREFIX)/lib
	install -m 0644 libemfiberthreads.a $(PREFIX)/lib/libemfiberthreads.a

clean:
	rm -rf obj
	rm -f libemfiberthreads.a
