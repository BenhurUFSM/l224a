#include "rede.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Cálculo dos valores da rede neural (produz as saídas à partir das entradas)

// função auxiliar para colocar o valor de um neurônio no intervalo -1:1
static float sigmoid(float x)
{
  return 1 / (1 + expf(-x));
}

void calcula_rede(Rede rede, float entradas[N_ENTRADAS], float saidas[N_SAIDAS])
{
// troca a linha abaixo para #if 1 para jogar sem rede
#if 0
  // calcula as saídas de forma direta, sem rede neural
  saidas[0] = 0;
  saidas[1] = 0;
  saidas[2] = 0;
  if (entradas[1] < entradas[0]) saidas[1] = 1;
  if (entradas[2] < entradas[0]) saidas[2] = 1;
  if (entradas[3] < 0.002) saidas[1] = 1;
#else
  if (rede == NULL) return;
  // copia os valores de entrada para os neurônios de entrada da rede
  for (int entrada = 0; entrada < N_ENTRADAS; entrada++) {
    Neuronio neuronio;
    grafo_valor_no(rede->grafo, PRIMEIRO_NO_ENTRADA + entrada, &neuronio);
    neuronio.valor = entradas[entrada];
    grafo_altera_valor_no(rede->grafo, PRIMEIRO_NO_ENTRADA + entrada, &neuronio);
  }
  // zera os valores de saída da rede (os neurônios de saída podem estar desconectados)
  for (int saida = 0; saida < N_SAIDAS; saida++) {
    Neuronio neuronio;
    grafo_valor_no(rede->grafo, PRIMEIRO_NO_SAIDA + saida, &neuronio);
    neuronio.valor = 0;
    grafo_altera_valor_no(rede->grafo, PRIMEIRO_NO_SAIDA + saida, &neuronio);
  }
  // calcula o valor de cada neurônio, na ordem da fila
  // o valor de um neurônio é o somatório do valor dos neurônios conectados às suas
  //   entradas multiplicados pelo peso da sinapse que faz essa conexão.
  // esse valor depois é somado ao viés e limitado pela função sigmoide.
  int no_origem, no_destino;
  Neuronio neuronio_origem, neuronio_destino;
  Sinapse sinapse;
  fila_inicia_percurso(rede->ordem, 0);
  while (fila_proximo(rede->ordem, &no_destino)) {
    // o valor das entradas é fornecido, não é calculado
    if (no_destino < N_ENTRADAS) continue;
    grafo_valor_no(rede->grafo, no_destino, &neuronio_destino);
    float soma = 0;
    grafo_arestas_que_chegam(rede->grafo, no_destino);
    while (grafo_proxima_aresta(rede->grafo, &no_origem, &sinapse)) {
      if (!sinapse.habilitada) continue;
      grafo_valor_no(rede->grafo, no_origem, &neuronio_origem);
      soma += neuronio_origem.valor * sinapse.peso;
    }
    neuronio_destino.valor = sigmoid(soma + neuronio_destino.vies);
    grafo_altera_valor_no(rede->grafo, no_destino, &neuronio_destino);
  }
  // copia os valores dos neurônios de saída para o vetor de saída
  for (int saida = 0; saida < N_SAIDAS; saida++) {
    Neuronio neuronio;
    grafo_valor_no(rede->grafo, PRIMEIRO_NO_SAIDA + saida, &neuronio);
    saidas[saida] = neuronio.valor;
  }
#endif

#ifdef LOG
  for (int i = 0; i < N_ENTRADAS; i++)
    fprintf(stderr, "%5.3f ", entradas[i]);
  fprintf(stderr, "| ");
  for(int i = 0; i < N_SAIDAS; i++)
    fprintf(stderr, "%5.3f ", saidas[i]);
  fprintf(stderr, "\n");
#endif
}

// cria uma rede vazia
Rede rede_cria_vazia()
{
  Rede self = malloc(sizeof(*self));
  if (self == NULL) return NULL;
  self->grafo = grafo_cria(sizeof(Neuronio), sizeof(Sinapse));
  if (self->grafo == NULL) {
    free(self);
    return NULL;
  }
  return self;
}

// cria uma rede neural a partir de um arquivo
Rede rede_cria(char *nome)
{
  FILE *arq = fopen(nome, "r");
  if (arq == NULL) return NULL;
  Rede self = rede_cria_vazia();
  if (self == NULL) {
    fclose(arq);
    return NULL;
  }
  int nnos, nentradas, nsaidas;
  if (fscanf(arq, "%d%d%d", &nnos, &nentradas, &nsaidas) != 3) goto err_arq;
  if (nentradas != N_ENTRADAS || nsaidas != N_SAIDAS) goto err_arq;
  for (int i = 0; i < nnos; i++) {
    Neuronio neuronio;
    if (fscanf(arq, "%d%f", &neuronio.id, &neuronio.vies) != 2) goto err_arq;
    neuronio.valor = 0;
    grafo_insere_no(self->grafo, &neuronio);
  }
  for (;;) {
    int no_origem, no_destino, habilitada;
    float peso;
    if (fscanf(arq, "%d%d%f%d", &no_origem, &no_destino, &peso, &habilitada) != 4) goto err_arq;
    if (no_origem == -1) break;
    Sinapse sinapse = { peso, habilitada == 1 };
    grafo_altera_valor_aresta(self->grafo, no_origem, no_destino, &sinapse);
  }
  self->ordem = grafo_ordem_topologica(self->grafo);
  fclose(arq);
  return self;

err_arq:
  fprintf(stderr, "erro na leitura do grafo de '%s'\n", nome);
  fclose(arq);
  grafo_destroi(self->grafo);
  free(self);
  return NULL;
}

// libera a memória ocupada por uma rede neural
void rede_destroi(Rede self)
{
  if (self == NULL) return;
  grafo_destroi(self->grafo);
  fila_destroi(self->ordem);
  free(self);
}
