#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados

struct queue * initqueue(struct queue *q)
{
    q = (struct queue *) malloc(sizeof(struct queue));
    q->head = NULL;
    q->tail = NULL;

    return q;
}

int isempty(struct queue *q)
{
    return (q->head == NULL && q->tail == NULL);
}

// inserindo processo no final da fila
void enqueue(struct queue *q, struct proc *p)
{
    // adicionando em uma fila vazia
    if (isempty(q))
    {
        // ajustando os ponteiros da fila
        q->head = p;
        q->tail = p;

        // ajustando os ponteiros do processo
        p->prev = NULL;
        p->next = NULL;
    }
    // adicionando ao final da fila
    else
    {
        // ajustando ponteiros do processo
        p->prev = q->tail;
        p->next = NULL;

        // ajustando os ponteiros da fila
        q->tail->next = p;
        q->tail = p;
    }

    // TODO: verificar se isto é suficiente para todos os escalonadores
    
    // OPERACOES DE CONTABILIZACAO DE TEMPO
    // ------------------------------------
    
    // realizando a contabilizacao de tempo no momento em que entra na fila
    if (q == ready)
    {
        // Realizando as estatisticas para o processo que 
        // entra na fila de aptos
        count_ready_in(p);
    }
    else if (q == ready2)
    {
        // entrar na fila2 tambem é uma fila de aptos
        count_ready_in(p);
    }
    else if (q == blocked)
    {
        // Realizando as estatisticas para o processo que 
        // entra na fila de bloqueados
        count_blocked_in(p);
    }
    else if (q == finished)
    {
        // Realizando as estatisticas para o processo que
        // entra na fila de finalizados
        count_finished_in(p);
    }
}


// removendo processo do inicio da fila
struct proc * dequeue(struct queue *q)
{    
    if (isempty(q))
    {
        printf("Fila vazia.\n");
        return NULL;
    }

    // obtendo o processo do inicio da fila
    struct proc *p;
    p = q->head;

    // andando a fila para proximo processo
    q->head = q->head->next;

    if (q->head == NULL)
    {
        q->tail = NULL;
    }

    // OPERACOES DE CONTABILIZACAO DE TEMPO
    // ------------------------------------
    
    // realizando a contabilizacao de tempo no momento em que o processo sai
    // da fila
    if (q == ready)
    {
        // Realizando as estatisticas para o processo que 
        // sai da fila de aptos
        count_ready_out(p);
    }
    else if (q == ready2)
    {
        // sair da fila2 tambem é uma fila de aptos
        count_ready_out(p);
    }
    else if (q == blocked)
    {
        // Realizando as estatisticas para o processo que 
        // sai da fila de bloqueados
        count_blocked_out(p);
    }

    return p;
}

struct proc * dequeue_bypid(struct queue * q, int pid)
{
    struct proc * aux;

    // removendo o processo selecionado da fila de aptos
    for ( aux = q->head; aux != NULL; aux = aux->next )
    {
        // removendo o processo - aux aponta para ele
        if (aux->pid == pid)
        {
            // tratando o processo prev
            if (aux->prev != NULL)
                aux->prev->next = aux->next;
            else
                // ajustando a fila
                q->head = aux->next;

            // tratando o processo next
            if (aux->next != NULL)
                aux->next->prev = aux->prev;
            else
                // ajustando a fila
                q->tail = aux->prev;

            aux->prev = NULL;
            aux->next = NULL;

            break;
        }
    }

    // OPERACOES DE CONTABILIZACAO DE TEMPO
    // ------------------------------------
    
    // realizando a contabilizacao de tempo no momento em que o processo sai
    // da fila
    if (q == ready)
    {
        // Realizando as estatisticas para o processo que 
        // sai da fila de aptos
        count_ready_out(aux);
    }
    else if (q == ready2)
    {
        // sair da fila2 tambem é uma fila de aptos
        count_ready_out(aux);
    }
    else if (q == blocked)
    {
        // Realizando as estatisticas para o processo que 
        // sai da fila de bloqueados
        count_blocked_out(aux);
    }

    return aux;
}

void printqueue(struct queue *q)
{
    if (isempty(q))
    {
        printf("Fila vazia.\n");
        return;
    }

    struct proc *aux;

    printf("\t____________ FILA _____________\n");

    printf("\t head: %d\n",q->head->pid);
    printf("\t tail: %d\n",q->tail->pid);
        
    printf("\t_______________________________\n");

    for(aux = q->head; aux != NULL; aux = aux->next)
    {
        printproc(aux);
        printf("\t_______________________________\n");
    }
    
}

int countqueue(struct queue *q)
{
    struct proc *aux;
    int num = 0;

    for(aux = q->head; aux != NULL; aux = aux->next)
        num++;

    return num;
}
