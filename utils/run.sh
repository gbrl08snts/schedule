# Define o nome do ficheiro de saída
OUTPUT="resultados_tme.csv"

# Cria o cabeçalho do arquivo CSV
echo "Algoritmo,Quantum,N_Processos,Rodada,TME" > $OUTPUT

ALGORITMOS=("fifo" "sjf" "ljf" "prio_static" "prio_dynamic" "prio_dynamic_quantum")

# Inicia as simulações
for ALGO in "${ALGORITMOS[@]}"; do
    echo "==========================================="
    echo "Iniciando simulações para: $ALGO"
    
    # Variação do Quantum (10, 20, 30)
    for Q in 10 20 30; do
             
        for NPROC in $(seq 10 10 100); do
            
            for RODADA in $(seq 1 30); do
                
                # O comando awk extrai apenas o valor numérico para o CSV
                VALOR_TME=$(../main_${ALGO} -n $NPROC -q $Q | grep "TME:" | awk '{print $2}')
                
                # Grava a linha no CSV
                echo "${ALGO},${Q},${NPROC},${RODADA},${VALOR_TME}" >> $OUTPUT
                
            done
        done
        echo " -> Concluído Quantum $Q para $ALGO"
    done
done

echo "==========================================="
echo "Todas as 5400 simulações concluídas! Dados salvos em utils/$OUTPUT"
