// Programa que mede tempo e memória ocupados por operações em uma fila
//
// Compila com uma implementação de fila:
//   gcc -o mede_fila mede_fila.c fila.c
// Executa passando o tamanho de cada dado como parametro:
//   ./mede_fila 4
// Dá para colocar a saída em um arquivo:
//   ./mede_fila 4 > saida
//
// A saída é assim:
//   
//   inicio, tamanho do dado = 4
//   =============================== 336 +336 kb  3 +3 ms
//no início do programa, ele tá com 336kb, o que é 336kb a mais que antes
//ele já executou por 3ms, o que é 3ms a mais que antes
//   
//   criacao
//   =============================== 4244 +3908 kb  4 +1 ms
//depois de criar a fila, ele tá usando 4244kb, 3908 a mais que antes
//já executou por 4ms, 1 a mais que antes
//   
//   parcial 9999, ult=1158
//   =============================== 4244 +0 kb  6 +2 ms
//na operação n. 9999 do primeiro conjunto de operacoes, a última remoção
//removeu o valor 1158. ainda tá ocupando 4244kb, demorou 2ms
//   
//   parcial 19999, ult=2273
//   =============================== 4244 +0 kb  10 +4 ms
//   
//   parcial 29999, ult=3378
//   =============================== 4244 +0 kb  17 +7 ms
//   
//   parcial 39999, ult=4462
//   =============================== 4244 +0 kb  25 +8 ms
//   
//   parcial 49999, ult=5583
//   =============================== 4244 +0 kb  35 +10 ms
//   
//   parcial 59999, ult=6716
//   =============================== 4244 +0 kb  47 +12 ms
//   
//   parcial 69999, ult=7850
//   =============================== 4244 +0 kb  61 +14 ms
//   
//   parcial 79999, ult=8968
//   =============================== 4244 +0 kb  77 +16 ms
//   
//   parcial 89999, ult=10096
//   =============================== 4244 +0 kb  96 +19 ms
//   
//   parcial 99999, ult=11242
//   =============================== 4244 +0 kb  122 +26 ms
//   
//   100000 90
//   =============================== 4244 +0 kb  122 +0 ms
//fim da primeira sequencia de operacoes, 100000 ops com 90% de inserções e 10%
//de remoções, ainda tá ocupando 4244kb, e já está executando há 122ms
//   
//   parcial 9999, ult=21211
//   =============================== 4244 +0 kb  341 +219 ms
//   
//   parcial 19999, ult=31205
//   =============================== 4244 +0 kb  500 +159 ms
//   
//   parcial 29999, ult=41209
//   =============================== 4244 +0 kb  628 +128 ms
//   
//   parcial 39999, ult=51319
//   =============================== 4244 +0 kb  731 +103 ms
//   
//   parcial 49999, ult=61330
//   =============================== 4244 +0 kb  819 +88 ms
//   
//   parcial 59999, ult=71322
//   =============================== 4244 +0 kb  890 +71 ms
//   
//   parcial 69999, ult=81284
//   =============================== 4244 +0 kb  945 +55 ms
//   
//   parcial 79999, ult=91286
//   =============================== 4244 +0 kb  981 +36 ms
//   
//   parcial 89999, ult=11752
//   =============================== 4244 +0 kb  1003 +22 ms
//   
//   parcial 99999, ult=99998
//   =============================== 4244 +0 kb  1010 +7 ms
//   
//   100000 10
//   =============================== 4244 +0 kb  1010 +0 ms
//   
//   fim
//   =============================== 336 -3908 kb  1011 +1 ms

#include "fila.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// o dado que sera inserido/removido a fila.
// dá para variar o tamanho para ver diferencas de comportamento
struct { int vet[256]; } dado;

// retorna a memória ocupada pelo programa, em kb
// executa o programa "ps" para conseguir essa informação.
// isso está longe de ter um bom desempenho, use com moderação.
#include <unistd.h>  // para getpid()
int memoria_ocupada()
{
  long mem = 0;
  FILE *arq;
  char cmd[100];

  sprintf(cmd, "ps -p %d -o size=", getpid());
  arq = popen(cmd, "r");
  if (arq != NULL) {
    fscanf(arq, "%ld", &mem);
    pclose(arq);
  }
  return mem;
}

// retorna o tempo de CPU do processo atual, em ms
#include <time.h>
int tempo_cpu_ms()
{
  return clock() / (CLOCKS_PER_SEC / 1000);
}

void imprime_medidas(char *texto)
{
  static int ultimo_m = 0;
  static int ultimo_t = 0;
  int m = memoria_ocupada();
  int t = tempo_cpu_ms();
  printf("\n%s\n", texto);
  printf("=============================== %d %+d kb  %d %+d ms\n",
         m, m - ultimo_m, t, t - ultimo_t);
  ultimo_m = m;
  ultimo_t = t;
}

// faz tantas operacoes na fila, tanto porcento inserções, o resto remoções,
//   imprimindo uma parcial de tempo e memória a cada tantas operações.
void faz_operacoes(Fila f, int n_ops, int percentual_inclusoes, int op_por_impr)
{
  int ult = -1;
  for (int op = 0; op < n_ops; op++) {
    dado.vet[0] = op;
    if (fila_vazia(f)) {
      fila_insere(f, &dado);
    } else {
      if (rand() % 100 < percentual_inclusoes) {
        fila_insere(f, &dado);
      } else {
        fila_remove(f, &dado);
        ult = dado.vet[0];
      }
    }
    if ((op + 1) % op_por_impr == 0) {
      char msg[100];
      sprintf(msg, "parcial %d, ult=%d", op, ult);
      imprime_medidas(msg);
    }
  }
}

int main(int argc, char *argv[]) {
  int tam_dado = 4;
  if (argc > 1) {
    tam_dado = atoi(argv[1]);
  }

  char msg[100];
  sprintf(msg, "inicio, tamanho do dado = %d", tam_dado);
  imprime_medidas(msg);

  Fila f = fila_cria(tam_dado);
  assert(f != NULL);
  imprime_medidas("criacao");
  faz_operacoes(f, 100000, 90, 10000);
  imprime_medidas("100000 90");
  faz_operacoes(f, 100000, 10, 10000);
  imprime_medidas("100000 10");
  fila_destroi(f);
  imprime_medidas("fim");
}
