#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

extern pthread_t sched_tid;

extern sigset_t set;
extern int snum;

extern sem_t sem_scheduling;

// Funcao para tratar algum signal recebido
void thread_handler(int sig)
{
    switch(sig)
    {
        // pause the process thread
        case SIGUSR1:
            //pause();
            break;
        // unpause a process thread
        case SIGUSR2:
            break;
    }
}

// Funcao que simula a troca de contexto mas, neste caso, a troca 
// de contexto sempre acontece do scheduling para um processo, e vice-versa
void context_switch(pthread_t tid)
{
    // Se é o scheduling, executa a thread 'tid' e pausa/dorme
    if (pthread_self() == sched_tid)
    {
        // "waking up" the 'tid' thread
        pthread_kill(tid, SIGUSR2);

        // "sleeping" scheduler thread
        sigwait(&set, &snum);  
    }
    // Se é outro processo, então libera a execução do scheduling
    // Em seguida, pausa aguardando um sinal
    else
    {
        // "waking up" scheduler thread
        pthread_kill(sched_tid, SIGUSR1);

        // "sleeping" current thread
        sigwait(&set, &snum);  
        // NOTE: using sigwait instead of pause to take more control on signals
    }
}
