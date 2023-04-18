# Problema da Montanha-Russa

# Contribuintes
Vitor Sorpile Geraldo (19102063)

# Compilação e Execução

-> Para compilar o código:

Abra o terminal na pasta "MontanhaRussa" e digite:
```bash
 make
   ```

-> Para executar o código:

   Abra o terminal na pasta "MontanhaRussa" e digite:
```bash
./MontanhaRussa
   ```
   

-> Para compilar executar o código em um comando:

   Abra o terminal na pasta "MontanhaRussa" e digite:
   ```bash
   make run
   ```
   

-> Para remover os arquivos objeto ("*.o"):

   Abra o terminal na pasta "MontanhaRussa" e digite:
   ```bash
   make clean
   ```

<!-- 
taskset -c 1 sudo valgrind --leak-check=full --show-leak-kinds=all -s ./Periodic_Thread 500 1 100000 SCHED_FIFO

taskset -c 1 sudo ./Periodic_Thread 500 1 100000 SCHED_FIFO 

-->