#ifndef _FILA_H_
#define _FILA_H_
#include <stdbool.h>

typedef struct _fila *Fila;

// funções que implementam as operações básicas de uma fila

// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila fila_cria(int tam_do_dado);
void fila_destroi(Fila self);

// diz se a fila está vazia
bool fila_vazia(Fila self);

// remove o dado no início da fila e copia para *pdado (se não for NULL)
void fila_remove(Fila self, void *pdado);

// insere o dado apontado por pdado no final da fila
void fila_insere(Fila self, void *pdado);

// funções que implementam operações complementares, que permitem acesso
//   a todos os elementos da fila. Durante um percurso, a fila não pode ser
//   alterada.

// inicia um percurso aos elementos da fila, a partir de uma posição
// se a posição for positiva, o percurso vai aumentando essa posição, até o fim da fila
// se a posição for negativa, o percurso vai diminuindo essa posição, até o início
//   0 é a posição do primeiro dado (aquele que está na fila há mais tempo)
//   1 é a posição do segundo dado, etc
//   além disso,
//   -1 é a posição do último dado (o que está na fila há menos tempo)
//   -2 é a posição do penúltimo dado, etc
void fila_inicia_percurso(Fila self, int pos_inicial);

// coloca o próximo dado do percurso em *pdado, se existir
// retorna true caso positivo, false caso o percurso tenha terminado
bool fila_proximo(Fila self, void *pdado);

#endif //_FILA_H_
