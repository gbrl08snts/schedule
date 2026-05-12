#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>

#include "queue.h"
#include "proc.h"
#include "proc_init.h"
#include "sched.h"
#include "utils.h"
#include "thread.h"
#include "proc_time.h"
#include "verbose.h"

// Número de processos
int NPROC = 10;

// Tempo máximo da execução total de um processo (microsegundos)
int MAX_TIME = 100;

// Tempo máximo da execução de um processo por entrada na CPU (microsegundos)
int QUANTUM = 20;

// Habilita ou desabilita o modo VERBOSE (DEBUG)
int VERBOSE = 0;

// Opção de fixar o valor da semente
int SEED = -1;

// Valor que define a chance de haver uma interrupção
double INTERRUPT_PROB = 0.5;

// Identificador da thread do scheduler
pthread_t sched_tid;

// Ponteiro para o processo que está rodando atualmente
struct proc * running = NULL;

// Ponteiros para as filas
struct queue * ready;       // fila de aptos
struct queue * ready2;      // segunda fila de aptos
struct queue * blocked;     // fila de bloqueados

// NOTE: usado apenas para contabilização final
struct queue * finished;    // fila de processos finalizados

// Global buffer para imprimir debug
char gbuffer[100];

// counter for the number of events of the system
int event_num = 1;

// Variaveis utilizadas para definir o conjunto de signals que as threads dos 
// processos irão aceitar
int snum;                                                              
sigset_t set;

int main (int argc, char *argv[])
{
    // tratando os parametros da linha de comando
    int opt;
    while ((opt = getopt(argc, argv, "n:q:s:vh")) != -1) {
        switch (opt) {
            // Imprimindo uma mensagem de ajuda (help)
            case 'h':
                printf("Ajuda:\n");
                printf("\t-n NPROC: Numero de processos.\n");
                printf("\t\tValido se NPROC >= 1. Default: NPROC = 10\n");
                printf("\t-q QUANTUM: Valor do QUANTUM.\n");
                printf("\t\tValido se QUANTUM >= 2. Default: QUANTUM = 20\n");
                printf("\t-s SEED: Valor fixo para a semente aleatória.\n");
                printf("\t\tValido se SEED != 1. Default: SEED = time(NULL)+PID\n");
                printf("\t-v: Habilita o modo 'verbose', para debug.\n");
                printf("\t-h: Exibe esta ajuda.\n");
                exit(0);
                break;

            // Obtem o número de processos NPROC informado na linha de comando
            case 'n':
                NPROC = atoi(optarg);
                // Verificando sua validade
                if( NPROC < 1)
                {
                    fprintf(stderr, "Informe um valor válido para os processos [NPROC >= 1]\n");
                    exit(1);
                }
                break;

            // Obtem o valor do QUANTUM informado na linha de comando
            case 'q':
                QUANTUM = atoi(optarg);
                if( QUANTUM < 2)
                {
                    fprintf(stderr, "Informe um valor válido para o QUANTUM [QUANTUM >= 2]\n");
                    exit(2);
                }
                break;

            // Fixando a SEED do srand
            case 's':
                SEED = atoi(optarg);
                if( SEED == -1)
                {
                    fprintf(stderr, "Informe um valor válido para o SEED [SEED != -1]\n");
                    exit(3);
                }
                break;

            // Modo 'verbose' para debug ativado
            case 'v':
                VERBOSE = 1;
                break;

            case '?':
                fprintf(stderr, "Erro de parametros!\n");
                fprintf(stderr, "Uso correto: %s [-n NPROC] [-q QUANTUM] [-v] [-h]\n",
                        argv[0]);
                exit(4);

            default:
                break;
        }
    }    
    
    // Parameters
    vprint("%s MAIN - Numero de PROCESSOS %d\n", event(), NPROC);
    
    vprint("%s MAIN - Valor do QUANTUM: %d\n", event(), QUANTUM);

    // definindo o conjunto de signals que as threads dos processos irão tratar
    sigemptyset(&set); 
    if(sigaddset(&set, SIGUSR1) == -1) 
    {
        perror("Sigaddset error");
        pthread_exit((void *)1);
    }
    if(sigaddset(&set, SIGUSR2) == -1) 
    {
        perror("Sigaddset error");
        pthread_exit((void *)1);
    }

    // NOTE: Marlison's FIX
    // blocking signals for all child threads (inheriting), only listen to
    // signals with sigwait, and the SO treats the sent signals as PENDING,
    // until the thread calls sigwait, not losing the signals sent before the
    // sigwait call
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
        perror("pthread_sigmask error");
        exit(3);
    }

    // Iniciando a semente do random
    if (SEED == -1)
        srand(time(NULL)+(int)getpid());
    else
        srand(SEED);
                
    vprint("%s MAIN - Iniciando filas\n", event());

    // Inicializando as filas
    ready = initqueue(ready);
    ready2 = initqueue(ready2);
    blocked = initqueue(blocked);
    finished = initqueue(finished);
    
    vprint("%s MAIN - Iniciando os processos\n", event());
    
    // Inicia os processos, inserindo-os na fila de aptos
    // NOTE: a fila é selecionada internamente à função, a partir das variaveis globais
    proc_init();

    vprint("%s MAIN - imprimindo fila 'ready':\n", event());
    if (VERBOSE)
        printqueue(ready);
    
    // printf("main: fila blocked:\n");
    // printqueue(blocked);

    // printproc(ready->head->next->next);

    vprint("%s MAIN - Iniciando o escalonador\n", event());
    
    // call scheduler
    start_scheduler();
    
    // printqueue(finished);

    // printf("%s MAIN - Liberando memória utilizada\n", event());

    // Finalizando os processos
    procend(finished);
    
    // printf("%s MAIN - Finalizando o simulador\n", event());
    
    return 0;
}
