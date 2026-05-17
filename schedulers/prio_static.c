#include <stdio.h>
#include <stdlib.h> // Necessário para a função rand()

#include "queue.h" 
#include "proc.h"  
#include "stats.h" 
#include "utils.h" 

extern struct queue * ready;    
extern struct queue * ready2;   
extern struct queue * blocked;  
extern struct queue * finished; 

extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    // 1. Tratar o processo atual que acabou de sair da execução
    if (current != NULL)
    {
        // 1.1 Avaliar a fila do processo de forma estática
        if (current->process_time_total > (0.3 * MAX_TIME)) {
            current->queue = 0; // Vai para a Fila 1 (ready)
        } else {
            current->queue = 1; // Vai para a Fila 2 (ready2)
        }

        printf("PRIO_STATIC: pid=%d process_time_total=%d MAX_TIME=%d -> fila %d\n",
               current->pid, current->process_time_total, MAX_TIME, current->queue);

        // 1.2 Enfileirar de acordo com o estado do processo
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
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // 2. Se não houver processos nas duas filas de aptos, retorna NULL
    if (isempty(ready) && isempty(ready2))
        return NULL;

    // Imprime o estado das duas filas antes de escolher
    printf("Fila ready1: ");
    struct proc * tmp = ready->head;
    while (tmp != NULL) {
        printf("[pid=%d ptt=%d] ", tmp->pid, tmp->process_time_total);
        tmp = tmp->next;
    }
    printf("\n");

    printf("Fila ready2: ");
    tmp = ready2->head;
    while (tmp != NULL) {
        printf("[pid=%d ptt=%d] ", tmp->pid, tmp->process_time_total);
        tmp = tmp->next;
    }
    printf("\n");

    struct proc * selected = NULL;

    // 3. Sorteio probabilístico para selecionar o próximo processo
    int chance = rand() % 100;

    if (chance < 70) 
    {
        // 70% de chance para a primeira fila
        if (!isempty(ready)) {
            selected = dequeue(ready);
            printf("PRIO_STATIC: chance=%d -> selecionou da fila 1: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready2);
            printf("PRIO_STATIC: chance=%d -> fila 1 vazia, fallback fila 2: pid=%d\n\n", chance, selected->pid);
        }
    } 
    else 
    {
        // 30% de chance para a segunda fila
        if (!isempty(ready2)) {
            selected = dequeue(ready2);
            printf("PRIO_STATIC: chance=%d -> selecionou da fila 2: pid=%d\n\n", chance, selected->pid);
        } else {
            selected = dequeue(ready);
            printf("PRIO_STATIC: chance=%d -> fila 2 vazia, fallback fila 1: pid=%d\n\n", chance, selected->pid);
        }
    }

    // 4. Modifica o estado do processo para executando e o retorna
    selected->state = RUNNING;
        
    return selected;
}