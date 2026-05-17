#include <stdio.h>
#include <stdlib.h>

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

// Tempo máximo da execução de um processo por entrada na CPU (microsegundos)
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected;

    if (current != NULL)
    {
        if (current->state != FINISHED)
        {
            // Calcula quanto do QUANTUM o processo 
            int tempo_usado = current->process_time;
            int metade = QUANTUM / 2;

            if (tempo_usado <= metade)
            {
                current->queue = 0; // usou <= 50% → fila 1
                printf("PRIO_DYN_Q: pid=%d usou %d/%d (<=50%%) -> fila 1\n",
                       current->pid, tempo_usado, QUANTUM);
            }
            else
            {
                current->queue = 1; // usou > 50% → fila 2
                printf("PRIO_DYN_Q: pid=%d usou %d/%d (>50%%) -> fila 2\n",
                       current->pid, tempo_usado, QUANTUM);
            }
        }

        switch (current->state)
        {
            case READY:
                if (current->queue == 0)
                    enqueue(ready, current);
                else
                    enqueue(ready2, current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                break;

            case FINISHED:
                enqueue(finished, current);
                printf("PRIO_DYN_Q: pid=%d FINALIZOU\n", current->pid);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Move processos desbloqueados 
    struct proc * aux = ready->head;
    while (aux!= NULL)
    {
        struct proc * next = aux->next;
        if (aux->queue == 1)
        {
            struct proc * tmp = dequeue_bypid(ready, aux->pid);
            enqueue(ready2, tmp);
            printf("PRIO_DYN_Q: pid=%d voltou de blocked -> movido para fila 2\n", tmp->pid);
        }
        aux = next;
    }

    if (isempty(ready) && isempty(ready2))
        return NULL;

    // Imprime o estado das duas filas antes de escolher
    printf("Fila ready1: ");
    struct proc * tmp = ready->head;
    while (tmp != NULL) {
        printf("[pid=%d] ", tmp->pid);
        tmp = tmp->next;
    }
    printf("\n");

    printf("Fila ready2: ");
    tmp = ready2->head;
    while (tmp != NULL) {
        printf("[pid=%d] ", tmp->pid);
        tmp = tmp->next;
    }
    printf("\n");

    int chance = rand() % 100;
    selected = NULL;

    if (chance < 70)
    {
        if (!isempty(ready)) {
            selected = dequeue(ready);
            printf("PRIO_DYN_Q: chance=%d -> selecionou da fila 1: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready2);
            printf("PRIO_DYN_Q: chance=%d -> fila 1 vazia, fallback fila 2: pid=%d\n\n", chance, selected->pid);
        }
    }
    else
    {
        if (!isempty(ready2)) {
            selected = dequeue(ready2);
            printf("PRIO_DYN_Q: chance=%d -> selecionou da fila 2: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready);
            printf("PRIO_DYN_Q: chance=%d -> fila 2 vazia, fallback fila 1: pid=%d\n\n", chance, selected->pid);
        }
    }

    selected->state = RUNNING;

    return selected;
}