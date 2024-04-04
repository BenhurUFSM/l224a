// Programa que usa filas
//
// Necessita os arquivos fila.h, fila.c, tela.h, tela.c, tecla.h, tecla.c
//
// para compilar este programa manualmente com gcc:
//   gcc -Wall -o cobra cobra.c fila.c tela.c tecla.c
//

#include "tecla.h"
#include "tela.h"

#include "fila.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// uma posição na tela
typedef struct {
  int lin;
  int col;
} posicao;

// tamanho de algo na tela
typedef struct {
  int nlin;
  int ncol;
} tamanho;

// um retângulo na tela (pos fica no canto superior esquerdo)
typedef struct {
  posicao pos;
  tamanho tam;
} retangulo;

// uma direção na tela
typedef enum { direita, esquerda, cima, baixo } direcao;

// uma cobra
//   composta pela posição de cada pedacinho e na direção em que se move
typedef struct {
  Fila corpo;
  posicao pos_cabeca;
  direcao dir;
} cobra;

// um obstáculo
typedef struct {
  posicao pos;
  char desenho[5]; // o maior código unicode ocupa 4 char em utf8
} obstaculo;

// o estado do jogo
typedef struct {
  retangulo tela;
  cobra aninha;
  int aumentando;
  Fila obstaculos;
  posicao premio;
  int pontos;
  enum { normal, terminando, terminado } estado;
} estado;

void desenha_cobra(Fila f) {
  posicao pos;
  fila_inicia_percurso(f, 0);
  fila_proximo(f, &pos);
  tela_lincol(pos.lin, pos.col);
  fputs("◦", stdout); // o rabo
  while (fila_proximo(f, &pos)) {
    tela_lincol(pos.lin, pos.col);
    fputs("○", stdout); // cada pedaço do corpo
  }
  tela_lincol(pos.lin, pos.col);
  fputs("⚇", stdout); // a cabeça
}

void desenha_obstaculos(Fila f) {
  tela_cor_letra(200, 0, 0);
  fila_inicia_percurso(f, 0);
  obstaculo obs;
  while (fila_proximo(f, &obs)) {
    tela_lincol(obs.pos.lin, obs.pos.col);
    fputs(obs.desenho, stdout);
  }
  tela_cor_normal();
}

void desenha_premio(posicao pos) {
  tela_lincol(pos.lin, pos.col);
  fputs("🍒", stdout);
}

posicao avanca_pos(posicao pos, direcao dir) {
  switch (dir) {
  case direita:
    pos.col++;
    break;
  case esquerda:
    pos.col--;
    break;
  case cima:
    pos.lin--;
    break;
  case baixo:
    pos.lin++;
    break;
  }
  return pos;
}

bool cobra_contem(Fila f, posicao pos) {
  posicao p;
  fila_inicia_percurso(f, 0);
  while (fila_proximo(f, &p)) {
    if (pos.lin == p.lin && pos.col == p.col)
      return true;
  }
  return false;
}

bool obstaculos_contem(Fila f, posicao pos) {
  obstaculo obs;
  fila_inicia_percurso(f, 0);
  while (fila_proximo(f, &obs)) {
    if (pos.lin == obs.pos.lin && pos.col == obs.pos.col)
      return true;
  }
  return false;
}

// desenha a borda de um retângulo
void borda_retangulo(retangulo r) {
  int li = r.pos.lin;
  int ci = r.pos.col;
  int nl = r.tam.nlin;
  int nc = r.tam.ncol;
  tela_lincol(li, ci);
  fputs("╭", stdout);
  for (int i = 0; i < nc - 2; i++) {
    fputs("─", stdout);
  }
  fputs("╮", stdout);
  for (int l = li + 1; l < li + nl - 1; l++) {
    tela_lincol(l, ci);
    fputs("│", stdout);
    tela_lincol(l, ci + nc - 1);
    fputs("│", stdout);
  }
  tela_lincol(li + nl - 1, ci);
  fputs("╰", stdout);
  for (int i = 0; i < nc - 2; i++) {
    fputs("─", stdout);
  }
  fputs("╯", stdout);
}

// preenche um retângulo com os caracteres em s
void preenche_retangulo(retangulo r, char *s) {
  for (int lin = r.pos.lin; lin < r.pos.lin + r.tam.nlin; lin++) {
    tela_lincol(lin, r.pos.col);
    for (int col = r.pos.col; col < r.pos.col + r.tam.ncol; col++) {
      fputs(s, stdout);
    }
  }
}

// retorna o número de colunas usadas para imprimir a string
// é o tamanho da string menos os caracteres de continuação utf8
int nchar(char *s) {
  // caracteres com código binário entre 0b10000000 e 0b10111111 são de
  // continuação em utf8
  char ci = 0b10000000;
  char cf = 0b10111111;
  int t = strlen(s);
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] >= ci && s[i] <= cf) {
      t--;
    }
  }
  return t;
}

// desenha o texto em txt de forma que ele termine em lin, col
void texto_esq(int lin, int col, char *txt) {
  int col_ini = col - nchar(txt) + 1;
  tela_lincol(lin, col_ini);
  fputs(txt, stdout);
}

// retorna uma posição aleatória dentro do retângulo
posicao sorteia_pos(retangulo r) {
  posicao pos;
  pos.lin = rand() % (r.tam.nlin - 2) + r.pos.lin + 1;
  pos.col = rand() % (r.tam.ncol - 2) + r.pos.col + 1;
  return pos;
}

void sorteia_obstaculos(estado *j) {
  while (!fila_vazia(j->obstaculos)) {
    fila_remove(j->obstaculos, NULL);
  }
  int nobs = 0;
  while (nobs != 4) {
    posicao pos = sorteia_pos(j->tela);
    if (!cobra_contem(j->aninha.corpo, pos) &&
        !obstaculos_contem(j->obstaculos, pos)) {
      obstaculo obs = {pos, "☠"};
      fila_insere(j->obstaculos, &obs);
      nobs++;
    }
  }
}

void sorteia_premio(estado *j) {
  posicao pos;
  do {
    pos = sorteia_pos(j->tela);
  } while (cobra_contem(j->aninha.corpo, pos) ||
           obstaculos_contem(j->obstaculos, pos));
  j->premio = pos;
}

void inicializa_jogo(estado *j) {
  // inicializa a tela gráfica
  tela_ini();
  tecla_ini();

  j->tela = (retangulo){{2, 1}, {tela_nlin() - 1, tela_ncol()}};
  j->aninha.corpo = fila_cria(sizeof(posicao));
  j->aninha.dir = cima;
  j->aninha.pos_cabeca = (posicao){tela_nlin() / 2, tela_ncol() / 2};
  fila_insere(j->aninha.corpo, &j->aninha.pos_cabeca);
  j->aumentando = 5;
  j->obstaculos = fila_cria(sizeof(obstaculo));
  sorteia_obstaculos(j);
  sorteia_premio(j);
  j->pontos = 0;
  j->estado = normal;
}

void desenha_tela(estado *j) {
  tela_limpa();

  tela_lincol(1, 1);
  printf("%d", j->pontos);

  // desenha o contorno da janela
  tela_cor_letra(255, 50, 200);
  borda_retangulo(j->tela);

  // desenha os objetos
  desenha_cobra(j->aninha.corpo);
  desenha_obstaculos(j->obstaculos);
  desenha_premio(j->premio);

  // faz aparecer na tela o que foi desenhado
  tela_atualiza();
}

void processa_teclado(estado *j) {
  char c = tecla_le_char();
  switch (c) {
  case 'u':
    j->aninha.dir = cima;
    break;
  case 'n':
    j->aninha.dir = esquerda;
    break;
  case 'e':
    j->aninha.dir = baixo;
    break;
  case 'i':
    j->aninha.dir = direita;
    break;
  }
}

void movimenta(estado *j) {
  if (j->estado == terminando) {
    fila_remove(j->aninha.corpo, NULL);
    if (fila_vazia(j->aninha.corpo)) {
      j->estado = terminado;
    }
    return;
  }
  // calcula a nova posicao da cabeça
  posicao pos = j->aninha.pos_cabeca;
  pos = avanca_pos(pos, j->aninha.dir);
  // vê se bateu em algo
  // TODO: ver se saiu da tela
  if (obstaculos_contem(j->obstaculos, pos) ||
      cobra_contem(j->aninha.corpo, pos)) {
    j->estado = terminando;
  }
  // a nova cabeça passa a fazer parte
  fila_insere(j->aninha.corpo, &pos);
  j->aninha.pos_cabeca = pos;
  // vê se acertou o prêmio
  if (pos.lin == j->premio.lin && pos.col == j->premio.col) {
    j->pontos += 17;
    j->aumentando += 5;
    sorteia_premio(j);
  }
  // se a cobra não tiver aumentando, tira o último pedaço
  if (j->aumentando == 0) {
    fila_remove(j->aninha.corpo, NULL);
  } else {
    j->aumentando--;
    j->pontos += 9;
  }
}

// A função principal
int main(void) {
  estado jogo;
  inicializa_jogo(&jogo);

  while (jogo.estado != terminado) {
    desenha_tela(&jogo);
    processa_teclado(&jogo);
    movimenta(&jogo);
  }

  // encerra a tela gráfica
  tela_fim();
  tecla_fim();

  return 0;
}
