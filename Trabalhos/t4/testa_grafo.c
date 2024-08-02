/*
 * programa simples de teste do grafo.
 * cria um grafo com um exemplo visto em aula, calcula a ordem topológica.
 * imprime os nós do grafo, as arestas que partem de cada nó, as arestas que chegam
 *   em cada nó, a ordem topológica dos nós.
 * abaixo está um exemplo de saída do programa (a saída pode ser diferente, porque as
 *   arestas podem ser entregues em outra ordem, e existem várias ordens topológicas
 *   válidas para esse grafo).
7 vértices:
0: 'sapato'
1: 'meia'
2: 'calça'
3: 'cinto'
4: 'casaco'
5: 'cueca'
6: 'camisa'
arestas (partindo):
origem 0('sapato')
origem 1('meia')
  destino 0(sapato): 'meia->sapato'
origem 2('calça')
  destino 0(sapato): 'calça->sapato'
  destino 3(cinto): 'calça->cinto'
origem 3('cinto')
origem 4('casaco')
origem 5('cueca')
  destino 2(calça): 'cueca->calça'
origem 6('camisa')
  destino 3(cinto): 'camisa->cinto'
  destino 4(casaco): 'camisa->casaco'
arestas (chegando):
destino 0('sapato')
  origem 1(meia): 'meia->sapato'
  origem 2(calça): 'calça->sapato'
destino 1('meia')
destino 2('calça')
  origem 5(cueca): 'cueca->calça'
destino 3('cinto')
  origem 2(calça): 'calça->cinto'
  origem 6(camisa): 'camisa->cinto'
destino 4('casaco')
  origem 6(camisa): 'camisa->casaco'
destino 5('cueca')
destino 6('camisa')
Ordem topológica:
1º nó 1('meia')
2º nó 5('cueca')
3º nó 6('camisa')
4º nó 2('calça')
5º nó 4('casaco')
6º nó 0('sapato')
7º nó 3('cinto')
*/

#include "grafo.h"
#include "fila.h"
#include <stdio.h>

Grafo cria_grafo()
{
  Grafo g = grafo_cria(7, 15);
  grafo_insere_no(g, "sapato");
  grafo_insere_no(g, "meia");
  grafo_insere_no(g, "calça");
  grafo_insere_no(g, "cinto");
  grafo_insere_no(g, "casaco");
  grafo_insere_no(g, "cueca");
  grafo_insere_no(g, "camisa");
  grafo_altera_valor_aresta(g, 1, 0, "meia->sapato");
  grafo_altera_valor_aresta(g, 2, 0, "calça->sapato");
  grafo_altera_valor_aresta(g, 5, 2, "cueca->calça");
  grafo_altera_valor_aresta(g, 2, 3, "calça->cinto");
  grafo_altera_valor_aresta(g, 6, 4, "camisa->casaco");
  grafo_altera_valor_aresta(g, 6, 3, "camisa->cinto");
  return g;
}

void imprime_grafo(Grafo g)
{
  printf("%d vértices:\n", grafo_nnos(g));
  for (int n = 0; n < grafo_nnos(g); n++) {
    char val_no[50];
    grafo_valor_no(g, n, val_no);
    printf("%d: '%s'\n", n, val_no);
  }
  printf("arestas (partindo):\n");
  for (int n = 0; n < grafo_nnos(g); n++) {
    char val_no[50];
    int n2;
    char val_no2[50];
    char val_aresta[50];
    grafo_valor_no(g, n, val_no);
    printf("origem %d('%s')\n", n, val_no);
    grafo_arestas_que_partem(g, n);
    while (grafo_proxima_aresta(g, &n2, val_aresta)) {
      grafo_valor_no(g, n2, val_no2);
      printf("  destino %d(%s): '%s'\n", n2, val_no2, val_aresta);
    }
  }
  printf("arestas (chegando):\n");
  for (int n = 0; n < grafo_nnos(g); n++) {
    char val_no[50];
    int n2;
    char val_no2[50];
    char val_aresta[50];
    grafo_valor_no(g, n, val_no);
    printf("destino %d('%s')\n", n, val_no);
    grafo_arestas_que_chegam(g, n);
    while (grafo_proxima_aresta(g, &n2, val_aresta)) {
      grafo_valor_no(g, n2, val_no2);
      printf("  origem %d(%s): '%s'\n", n2, val_no2, val_aresta);
    }
  }
}

void imprime_fila(Fila f, Grafo g)
{
  int no;
  int ordem = 1;
  printf("Ordem topológica:\n");
  fila_inicia_percurso(f, 0);
  while (fila_proximo(f, &no)) {
    char val_no[50];
    grafo_valor_no(g, no, val_no);
    printf("%dº nó %d('%s')\n", ordem, no, val_no);
    ordem++;
  }
}

int main()
{
  Grafo g = cria_grafo();
  imprime_grafo(g);
  Fila f = grafo_ordem_topologica(g);
  imprime_fila(f, g);
  fila_destroi(f);
  grafo_destroi(g);
  return 0;
}
