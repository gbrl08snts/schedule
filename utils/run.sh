# executando o comando abaixo 100 vezes
for i in $(seq 100)
do
    # filtrando para exibir apenas a linha com a expressao 'TME'
    ../main_fifo -n 100 -q 20 | grep TME
done
