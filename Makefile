# defining the compiler
CC = gcc

# defining the include directory (.h)
INC = -I./include

# defining the flags
CFLAGS = -Wall -g $(INC)

# main objects for all executables
OBJS = build/main.o \
	   build/proc.o \
	   build/queue.o \
	   build/sched.o \
	   build/utils.o \
	   build/proc_time.o \
	   build/thread.o \
	   build/stats.o \
	   build/verbose.o

default: fifo

all: fifo sjf ljf prio_static prio_dynamic prio_dynamic_quantum

setup:
	mkdir -p build

fifo: $(OBJS) build/fifo.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/fifo.o build/proc_init.o build/proc_interrupt.o -o main_fifo

sjf: $(OBJS) build/sjf.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/sjf.o build/proc_init.o build/proc_interrupt.o -o main_sjf

ljf: $(OBJS) build/ljf.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/ljf.o build/proc_init.o build/proc_interrupt.o -o main_ljf

prio_static: $(OBJS) build/prio_static.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/prio_static.o build/proc_init.o build/proc_interrupt.o -o main_prio_static

prio_dynamic: $(OBJS) build/prio_dynamic.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/prio_dynamic.o build/proc_init.o build/proc_interrupt.o -o main_prio_dynamic

prio_dynamic_quantum: $(OBJS) build/prio_dynamic_quantum.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/prio_dynamic_quantum.o build/proc_init.o build/proc_interrupt.o -o main_prio_dynamic_quantum

build/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o build/main.o

build/proc.o: src/proc.c include/proc.h
	$(CC) $(CFLAGS) -c src/proc.c -o build/proc.o

build/proc_init.o: src/proc_init.c include/proc_init.h
	$(CC) $(CFLAGS) -c src/proc_init.c -o build/proc_init.o

build/proc_interrupt.o: src/proc_interrupt.c include/proc_interrupt.h
	$(CC) $(CFLAGS) -c src/proc_interrupt.c -o build/proc_interrupt.o

build/queue.o: src/queue.c include/queue.h
	$(CC) $(CFLAGS) -c src/queue.c -o build/queue.o

build/sched.o: src/sched.c include/sched.h
	$(CC) $(CFLAGS) -c src/sched.c -o build/sched.o

build/utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -o build/utils.o

build/proc_time.o: src/proc_time.c include/proc_time.h
	$(CC) $(CFLAGS) -c src/proc_time.c -o build/proc_time.o

build/thread.o: src/thread.c include/thread.h
	$(CC) $(CFLAGS) -c src/thread.c -o build/thread.o

build/stats.o: src/stats.c include/stats.h
	$(CC) $(CFLAGS) -c src/stats.c -o build/stats.o

build/verbose.o: src/verbose.c include/verbose.h
	$(CC) $(CFLAGS) -c src/verbose.c -o build/verbose.o

build/fifo.o: schedulers/fifo.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/fifo.c -o build/fifo.o

build/sjf.o: schedulers/sjf.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/sjf.c -o build/sjf.o

build/ljf.o: schedulers/ljf.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/ljf.c -o build/ljf.o

build/prio_static.o: schedulers/prio_static.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_static.c -o build/prio_static.o

build/prio_dynamic.o: schedulers/prio_dynamic.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_dynamic.c -o build/prio_dynamic.o

build/prio_dynamic_quantum.o: schedulers/prio_dynamic_quantum.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_dynamic_quantum.c -o build/prio_dynamic_quantum.o

clean:
	rm -f build/*.o
	rm -f main_fifo
	rm -f main_sjf
	rm -f main_ljf
	rm -f main_prio_static
	rm -f main_prio_dynamic
	rm -f main_prio_dynamic_quantum

