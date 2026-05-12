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
// as estatisticas finais, voce deve inserir os processos que finalizam nesta
// fila finished

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    // este é o processo que será selecionado pelo escalonador e retornado pela
    // função ao final
    struct proc * selected; 

    /*
     *   Tratando o processo que está atualmente executando
     */

    // TODO: Neste ponto o escalonador tem a variavel 'current' apontando 
    // TODO: para o processo que acabou de sair da execução, é necessário
    // TODO: verificar o estado em que este processo está, e fazer o seu 
    // TODO: devido tratamento: 
    //          a) verificar o seu estado
    //          b) adicioná-lo na fila adequada a seu estado

    // Para tratar o caso do início do sistema, quando nao tem ninguem rodando
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            case READY:

                // Adicionando no final da fila de aptos
                enqueue(ready, current);
                
                break;

            // Caso o processo tenha saído como 'bloqueado', é porque fez uma E/S
            case BLOCKED:

                // Adicionando no final da fila de bloqueados
                enqueue(blocked, current);
                
                break;

            // Caso o processo tenha finalizado, estado 'finished'
            case FINISHED:

                // Adicionando-o na fila de finalizados
                enqueue(finished, current);

                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready))
    {
        // printf("== sched FIFO: fila READY is empty\n");
        return NULL;
    }

    // TODO: Neste ponto, é preciso implementar a estrategia do algoritmo
    // TODO: utilizado, para selecionar um processo a ser selecionado.

    // Estratégia para o FIFO:

    // printf(" FILA READY ANTES\n");
    // printqueue(ready);

    // Para o RR, é suficiente pegar o primeiro processo da fila de aptos para executar
    selected = dequeue(ready);

    // printf(" FILA READY DEPOIS\n");
    // printqueue(ready);

    // WARNING: ATENÇÃO: para remover um processo de uma fila, é necessário fazer 
    // isto utilizando as funções disponibilizadas no arquivo "queue.h", pois
    // nelas há a contabilização das estatisticas de tempo do processo, que
    // serão usadas para o cálculos das estatísticas finais.
    // Caso voce precise implementar algo a mais do que estas funções, atente
    // às seções de "OPERACOES DE CONTABILIZACAO DE TEMPO" nestas funçoes
    
    // NOTE: As funções disponíveis para inserir e retirar um processo de uma fila são:
    // - void enqueue(struct queue *q, struct proc *p)
    //      -> que inserir um processo 'p' na fila 'q'
    // - struct proc * dequeue(struct queue *q)
    //      -> que remove o processo do inicio da fila 'q'
    // - struct proc * dequeue_bypid(struct queue * q, int pid)
    //      -> que remove o processo de acordo com o seu 'pid' da fila 'q'
    
    // TODO: Neste ponto, é necessário alterar o estado do processo para executando

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}

