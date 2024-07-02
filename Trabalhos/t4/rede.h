#ifndef _REDE_H_
#define _REDE_H_

// Uma rede neural simples
//
// A rede é um grafo de nós (neurônios) interligados por arestas (sinapses).
// Alguns desses nós são nós de entrada, outros são nós de saída; os demais são nós
//   intermediários.
// Cada nó tem um valor associado, que pode ser calculado pela rede ou ser um valor
//   de entrada (no caso dos nós de entrada).
// Cada aresta tem um valor associado, chamado peso. As arestas são direcionais, ligam
//   um nó de origem a um nó de destino.
// O valor de um nó é calculado pelo somatório dos valores de seus nós antecessores
//   multiplicado pelo peso da aresta que o liga a esse antecessor. Para uniformizar
//   esses valores, é usada uma função "sigmoide", com um parâmetro "viés", que é também
//   armazenado no nó.
// Os valores calculados nos nós de saída são os valores de saída da rede.
//
// O cálculo do valor de um nó é dependente dos valores dos nós que o precedem, logo
//   necessitamos de uma ordem topológica para realizar esses cálculos.
// Para facilitar a identificação dos nós de entrada e saída do grafo, os nós de entrada
//   são numerados de 0 a N_ENTRADAS-1, os nós de saída entre N_ENTRADAS e 
//   N_ENTRADAS+N_SAIDAS-1 e os nós intermediários à partir de N_ENTRADAS+N_SAIDAS.

// número de nós de entrada e de saída na rede neural
#define N_ENTRADAS 6
#define N_SAIDAS 3

#define PRIMEIRO_NO_ENTRADA 0
#define PRIMEIRO_NO_SAIDA (0+N_ENTRADAS)
#define PRIMEIRO_NO_INTERMEDIARIO (N_ENTRADAS+N_SAIDAS)

#include "grafo.h"
#include "fila.h"

typedef struct _rede *Rede;

// as estruturas de rede, neurônio e sinapse estão abertas aqui para simplificar a 
// implementação do treinador de rede. Essas estruturas não devem ser acessadas
// diretamente a não ser pelo treinador.

// Rede neural.
// Contém um grafo: nos vértices tem neurônios e nas arestas sinapses.
// Os primeiros N_ENTRADAS nós contêm os neurônios de entrada, os N_SAIDAS seguintes
//   contêm os de saída, os demais contêm os intermediários.
// Contém também uma fila, com os números dos nós na ordem em que têm que ser calculados.
struct _rede {
  Grafo grafo;
  Fila ordem;
};

// Um neurônio tem uma identificação, e um viés, que é utilizado no cálculo de seu valor.
typedef struct {
  float vies;
  float valor;
  int id;
} Neuronio;

// Uma sinapse contém um peso usado no cálculo do valor do neurônio de chegada, e pode ser
// desabilitada
typedef struct {
  float peso;
  bool habilitada;
} Sinapse;


// cria uma rede neural a partir de um arquivo
Rede rede_cria(char *nome);

// cria uma rede vazia
Rede rede_cria_vazia();

// libera a memória ocupada por uma rede neural
void rede_destroi(Rede self);

// recalcula os valores dos nós da rede, à partir dos valores fornecidos para as entradas;
// copia os valores dos nós de saída para o vetor saidas
void calcula_rede(Rede rede, float entradas[N_ENTRADAS], float saidas[N_SAIDAS]);

// funções que devem ser implementadas para calcular a pontuação de uma rede
// (estão em cobra.c)
int joga_sem_tela(Rede rede, int tam_ini, int max_passos);
int joga_com_tela(Rede rede);

#endif // _REDE_H_
