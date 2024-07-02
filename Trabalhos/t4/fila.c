#include "fila.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEM 1000

struct _fila {
  int n_elem;
  int tam_dado;
  int pos_percurso;
  void *espaco;
};

// funções que implementam as operações básicas de uma fila

// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila fila_cria(int tam_do_dado) {
  Fila self = malloc(sizeof(struct _fila));
  if (self != NULL) {
    self->espaco = malloc(MAX_ELEM * tam_do_dado);
    if (self->espaco != NULL) {
      self->n_elem = 0;
      self->tam_dado = tam_do_dado;
    } else {
      free(self);
      self = NULL;
    }
  }
  return self;
}

void fila_destroi(Fila self) {
  free(self->espaco);
  free(self);
}

// calcula o valor do ponteiro para o elemento na posição pos da fila
//   retorna NULL se não existir elemento nessa posição da fila
static void *calcula_ponteiro(Fila self, int pos)
{
  // TODO: suporte a pos negativa

  // se a posição fornecida não estiver dentro da fila, retorna NULL
  if (pos < 0 || pos >= self->n_elem) return NULL;
  
  // não conhecemos o tipo do dado do usuário, só o tamanho em bytes.
  // temos um ponteiro para void apontando para o início do vetor.
  // o dado desejado está tantos bytes além do início do vetor.
  // para calcular a posição do dado, temos que converter o ponteiro void
  //   para um ponteiro para dados do tamanho de um byte (char).
  // a posição do dado no vetor é a mesma posição do dado no vetor (isso não
  //   será mais válido com vetor circular).
  int deslocamento_em_bytes = pos * self->tam_dado;
  char *ptr_em_bytes_para_o_inicio = (char *)(self->espaco);
  char *ptr_em_bytes_para_o_dado = ptr_em_bytes_para_o_inicio + deslocamento_em_bytes;
  void *ptr = (void *)ptr_em_bytes_para_o_dado;
  return ptr;
}

bool fila_vazia(Fila self) { return self->n_elem == 0; }

void fila_remove(Fila self, void *pdado) {
  // vai remover o primeiro dado da fila, obtém um ponteiro para ele
  void *ptr = calcula_ponteiro(self, 0);
  // se não conseguiu, é porque a fila está vazia, aborta o programa
  assert(ptr != NULL);
  // copia o dado para o usuário
  if (pdado != NULL) {
    memmove(pdado, ptr, self->tam_dado);
  }
  // obtém a posição do novo primeiro elemento (se não houver, será NULL)
  ptr = calcula_ponteiro(self, 1);
  if (ptr != NULL) {
    // ptr aponta para o segundo elemento da fila
    //   (que vira o primeiro)
    // copia os dados que sobraram para o início do espaço
    //   (não precisaria fazer isso com vetor circular)
    memmove(self->espaco, ptr, self->tam_dado * (self->n_elem - 1));
  }
  self->n_elem--;
}

void fila_insere(Fila self, void *pdado) {
  // nossa fila tem um número máximo de elementos (essa limitação deve ser removida)
  assert(self->n_elem < MAX_ELEM);
  // incrementa o número de elementos, para que a próxima posição seja válida e seu endereço possa ser calculado
  self->n_elem++;
  // calcula a posição onde o dado deve ser colocado
  void *ptr = calcula_ponteiro(self, self->n_elem - 1);
  // copia o dado do usuário para o vetor
  memmove(ptr, pdado, self->tam_dado);
}


void fila_inicia_percurso(Fila self, int pos_inicial)
{
  // o percurso iniciará na posição fornecida
  self->pos_percurso = pos_inicial;
}

bool fila_proximo(Fila self, void *pdado)
{
  // calcula o endereço onde está o próximo dado do percurso
  void *ptr = calcula_ponteiro(self, self->pos_percurso);
  // caso não existir dado nessa posição, o percurso terminou
  if (ptr == NULL) return false;
  // copia o dado para o ponteiro do usuário
  if (pdado != NULL) {
    memmove(pdado, ptr, self->tam_dado);
  }
  // calcula a posição do próximo dado do percurso
  if (self->pos_percurso < 0) {
    self->pos_percurso--;
  } else {
    self->pos_percurso++;
  }
  // o dado foi encontrado, o percurso ainda não terminou
  return true;
}
