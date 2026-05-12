# sched_sim - Scheduler Simulator

Simulador de Escalonador baseado em POSIX threads.

Este é um simulador de escalonador educacional, a ser utilizado na disciplina
DCT2101 - Sistemas Operacionais, do BSI, CERES-UFRN.

Esta versão possui a implementação do algoritmo de escalonamento FIFO, sendo
requisitados a implementação dos demais algoritmos de escalonamento.

## Compilação

Para compilação de todos os códigos:

```
make all
```

Para compilação individual dos escalonadores:

FIFO:

```
make fifo
```

SJF:

```
make sjf
```

LJF:

```
make ljf
```

PRIO STATIC:

```
make prio_static
```

PRIO DYNAMIC:

```
make prio_dynamic
```

Serão gerados arquivos executáveis para cada um dos escalonadores.

## Execução

Para executar os escalonadores:

```
./main_fifo -n 10 -q 20
```

Onde, os parâmetros significam:

- **-n NPROC**: Número de processos (caso não informado, o valor *default* é 10)
- **-q QUANTUM**: Valor do QUANTUM (caso não informado, o valor *default* é 20)

Para análise da execução:

- **-s SEED**: Para fixar um valor da semente da geração de números
pseudo-aleatórios (caso não informado, o valor *default* é *time(NULL)+PID*).
- **-v**: Modo de visualização da execução, para DEBUG.

Para mais detalhes sobre os parâmetros:

- **-h**: Para exibir um menu de ajuda.


