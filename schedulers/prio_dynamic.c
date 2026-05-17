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

struct proc * scheduler(struct proc * current)
{
    struct proc * selected;

    if (current != NULL)
    {
        switch (current->state)
        {
            // Saiu por preempção → volta para fila 1
            case READY:
                current->queue = 0;
                enqueue(ready, current);
                printf("PRIO_DYNAMIC: pid=%d saiu por PREEMPCAO -> fila 1\n", current->pid);
                break;

            // Saiu por E/S → vai para blocked, marcado para fila 2 ao voltar
            case BLOCKED:
                current->queue = 1;
                enqueue(blocked, current);
                printf("PRIO_DYNAMIC: pid=%d saiu por E/S -> blocked (voltara para fila 2)\n", current->pid);
                break;

            case FINISHED:
                enqueue(finished, current);
                printf("PRIO_DYNAMIC: pid=%d FINALIZOU\n", current->pid);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Move processos desbloqueados que voltaram para ready mas pertencem à fila 2
    struct proc * p = ready->head;
    while (p != NULL)
    {
        struct proc * next = p->next;
        if (p->queue == 1)
        {
            struct proc * tmp = dequeue_bypid(ready, p->pid);
            enqueue(ready2, tmp);
            printf("PRIO_DYNAMIC: pid=%d voltou de blocked -> movido para fila 2\n", tmp->pid);
        }
        p = next;
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
            printf("PRIO_DYNAMIC: chance=%d -> selecionou da fila 1: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready2);
            printf("PRIO_DYNAMIC: chance=%d -> fila 1 vazia, fallback fila 2: pid=%d\n\n", chance, selected->pid);
        }
    }
    else
    {
        if (!isempty(ready2)) {
            selected = dequeue(ready2);
            printf("PRIO_DYNAMIC: chance=%d -> selecionou da fila 2: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready);
            printf("PRIO_DYNAMIC: chance=%d -> fila 2 vazia, fallback fila 1: pid=%d\n\n", chance, selected->pid);
        }
    }

    selected->state = RUNNING;

    return selected;
}
