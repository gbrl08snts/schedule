#include <stdio.h>

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
            case READY:
                enqueue(ready, current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                break;

            case FINISHED:
                enqueue(finished, current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    if (isempty(ready))
        return NULL;

    // Pega o primeiro processo da fila.
    struct proc * longest = ready->head;
    struct proc * aux = longest->next;

    // Procura o processo com maior remaining_time.
    while (aux != NULL)
    {
        if (aux->remaining_time > longest->remaining_time)
            longest = aux;

        aux = aux->next;
    }

    // Imprime todos os processos na fila antes de escolher
    printf("Fila ready: ");
    struct proc * tmp = ready->head;
    while (tmp != NULL) {
        printf("[pid=%d rt=%d] ", tmp->pid, tmp->remaining_time);
        tmp = tmp->next;
    }
    printf("\n");

    // Remove da fila o processo escolhido.
    printf("LJF selecionou pid=%d remaining_time=%d\n\n", longest->pid, longest->remaining_time);
    selected = dequeue_bypid(ready, longest->pid);

    selected->state = RUNNING;

    return selected;
}