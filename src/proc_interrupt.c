#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos

// para tratar o que fazer com o processo ao ocorrer a 
// interrupçao que define sua saída da fila de bloqueados
void proc_interrupt(struct proc * p)
{
    // insere o processo no final da sua fila de aptos
    // OBS: de acordo com a variavel 'queue', ele pode ir pra uma fila ou
    // outra
    if (p->queue == 0)
        enqueue(ready, p);
    else
        enqueue(ready2, p);

    // alterando o status para apto
    p->state = READY;

    // Realizando as estatisticas para o processo que 
    // entra na fila de aptos
    count_ready_in(p);
}

