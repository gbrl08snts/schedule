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

    struct proc * selected = NULL;

    // 3. Sorteio probabilístico para selecionar o próximo processo
    int chance = rand() % 100;

    if (chance < 70) 
    {
        // 70% de chance para a primeira fila
        if (!isempty(ready)) {
            selected = dequeue(ready);
        } else {
            // Fallback: se a fila 1 estiver vazia, pega da fila 2
            selected = dequeue(ready2);
        }
    } 
    else 
    {
        // 30% de chance para a segunda fila
        if (!isempty(ready2)) {
            selected = dequeue(ready2);
        } else {
            // Fallback: se a fila 2 estiver vazia, pega da fila 1
            selected = dequeue(ready);
        }
    }

    // 4. Modifica o estado do processo para executando e o retorna
    selected->state = RUNNING;
        
    return selected;
}