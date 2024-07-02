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

#include "rede.h"

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

posicao retangulo_centro(retangulo r) {
  return (posicao){ r.pos.lin+r.tam.nlin/2, r.pos.col+r.tam.ncol/2 };
}

bool dentro_do_retangulo(posicao p, retangulo r)
{
  if (p.lin <= r.pos.lin) return false;
  if (p.col <= r.pos.col) return false;
  if (p.lin >= r.pos.lin+r.tam.nlin-1) return false;
  if (p.col >= r.pos.col+r.tam.ncol-1) return false;
  return true;
}

// uma direção na tela
typedef enum { direita, esquerda, cima, baixo } direcao;

typedef enum { reto, pra_direita, pra_esquerda } lado_da_curva;

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
  posicao maçã;
  int pontos;
  enum { normal, terminando, terminado } estado;
  int passos;
  int passo_ultima_maçã;
  int max_passos;
} estado;

void desenha_cobra(Fila f, bool terminando) {
  if (terminando) {
    tela_cor_letra(200, 30, 30);
  } else {
    tela_cor_letra(150, 150, 0);
  }
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
  fputs("🍎", stdout);
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
  j->maçã = pos;
}

void inicializa_tela() {
  tela_ini();
  tecla_ini();
}

void inicializa_jogo(estado *j, int tam_inicial_da_cobra, int max_passos)
{
  j->tela = (retangulo){{2, 1}, {24, 50}};
  j->aninha.corpo = fila_cria(sizeof(posicao));
  j->aninha.dir = cima;
  j->aninha.pos_cabeca = retangulo_centro(j->tela);
  fila_insere(j->aninha.corpo, &j->aninha.pos_cabeca);
  j->aumentando = tam_inicial_da_cobra;
  if (j->aumentando <= 0) j->aumentando = 1;
  j->obstaculos = fila_cria(sizeof(obstaculo));
  sorteia_obstaculos(j);
  sorteia_premio(j);
  j->pontos = 0;
  j->estado = normal;
  j->passos = 0;
  j->passo_ultima_maçã = 0;
  j->max_passos = max_passos;
}

void finaliza_tela() {
  tela_fim();
  tecla_fim();
}

void finaliza_jogo(estado *j) {
  fila_destroi(j->aninha.corpo);
  fila_destroi(j->obstaculos);
}

void desenha_tela(estado *j) {
  tela_limpa();

  // desenha o contorno da janela
  tela_cor_letra(255, 50, 200);
  borda_retangulo(j->tela);

  // desenha os objetos
  desenha_obstaculos(j->obstaculos);
  desenha_premio(j->maçã);
  desenha_cobra(j->aninha.corpo, j->estado == terminando);

  tela_lincol(1, 1);
  printf("%d", j->pontos);

  // faz aparecer na tela o que foi desenhado
  tela_atualiza();
}

// a rede neural pede para virar à esquerda ou à direita, em vez de direção
//   absoluta como com o teclado
void faz_curva(estado *j, lado_da_curva lado)
{
  // qual a próxima direção se está indo para tal direção e faz tal curva?
  direcao prox_dir[] = { 
    [cima]=direita, [direita]=baixo, [baixo]=esquerda, [esquerda]=cima
  };
  direcao prox_esq[] = { 
    [cima]=esquerda, [direita]=cima, [baixo]=direita, [esquerda]=baixo
  };
  if (lado == pra_esquerda) {
    j->aninha.dir = prox_esq[j->aninha.dir];
  } else if (lado == pra_direita) {
    j->aninha.dir = prox_dir[j->aninha.dir];
  } // se for reto, não muda
}

// interface com a rede neural


// função auxiliar para cálculo de distâncias.
// a cobra está na posição pcob e tem um objeto na posição pobj
// se esse objeto estiver na linha de visão da cobra para algum lado (d,e,c,b)
//   e mais próximo do que a distância que tem nos ponteiros, ajusta a distância
static void diminui_distancias(posicao pcob, posicao pobj,
                        int *dd, int *de, int *dc, int *db)
{
  float dcol = pobj.col - pcob.col;
  float dlin = pobj.lin - pcob.lin;
  if (dlin == 0 && dcol >= 0 &&  dcol < *dd) *dd =  dcol;
  if (dlin == 0 && dcol <= 0 && -dcol < *de) *de = -dcol;
  if (dcol == 0 && dlin >= 0 &&  dlin < *db) *db =  dlin;
  if (dcol == 0 && dlin <= 0 && -dlin < *dc) *dc = -dlin;
}

// preenche o vetor de entradas com valores que serão as entradas da rede neural
void calcula_entradas(estado *j, float entradas[N_ENTRADAS])
{
  // a rede usa como entrada as distâncias na linha de visão da cobra, direto
  //   para a frente, para a direita e para a esquerda.
  // 0,1,2 - distância até a maçã, na direção da cobra, à direita e à esquerda
  // 3,4,5 - distância até um obstáculo ou parede ou corpo, na direção da cobra,
  //         à direita e à esquerda
  // O valor da distância é entre 0 e 1: 0 para muito perto, 1 para bem longe
  // A função calcula a distância em posições de locomoção da cobra, e depois
  //   divide por 1000.
  // As distâncias são calculadas em direções absolutas (d,e,c,b) e depois convertidas
  //   em relação à direção da cobra.

  int ncol = j->tela.tam.ncol;
  int nlin = j->tela.tam.nlin;
  // distâncias até a maçã
  int md = ncol, me = ncol, mc = nlin, mb = nlin;
  md = me = mc = mb = 1000; // bem longe
  diminui_distancias(j->aninha.pos_cabeca, j->maçã, &md, &me, &mc, &mb);
  // distância até obstáculo
  int od, oe, oc, ob;
  // até a parede
  oe = j->aninha.pos_cabeca.col - j->tela.pos.col;
  od = ncol - oe - 1;
  oc = j->aninha.pos_cabeca.lin - j->tela.pos.lin;
  ob = nlin - oc - 1;
  // até algum obstáculo
  obstaculo obs;
  fila_inicia_percurso(j->obstaculos, 0);
  while (fila_proximo(j->obstaculos, &obs)) {
    diminui_distancias(j->aninha.pos_cabeca, obs.pos, &od, &oe, &oc, &ob);
  }
  // até algum pedaço do corpo (menos a cabeça)
  posicao pos;
  fila_inicia_percurso(j->aninha.corpo, 0);
  while (fila_proximo(j->aninha.corpo, &pos)) {
    if (pos.lin != j->aninha.pos_cabeca.lin || pos.col != j->aninha.pos_cabeca.col) {
      diminui_distancias(j->aninha.pos_cabeca, pos, &od, &oe, &oc, &ob);
    }
  }
  // converte em 0--1, relativo à direção da cobra, coloca no vetor de entradas
  float fh = 0.001; // fator de conversão de distâncias horizontais
  float fv = 0.001; // fator de conversão de distâncias verticais
  switch (j->aninha.dir) {
    case direita:
      entradas[0] = md*fh; entradas[1] = mb*fv; entradas[2] = mc*fv;
      entradas[3] = od*fh; entradas[4] = ob*fv; entradas[5] = oc*fv;
      break;
    case esquerda:
      entradas[0] = me*fh; entradas[1] = mc*fv; entradas[2] = mb*fv;
      entradas[3] = oe*fh; entradas[4] = oc*fv; entradas[5] = ob*fv;
      break;
    case cima:
      entradas[0] = mc*fv; entradas[1] = md*fh; entradas[2] = me*fh;
      entradas[3] = oc*fv; entradas[4] = od*fh; entradas[5] = oe*fh;
      break;
    case baixo:
      entradas[0] = mb*fv; entradas[1] = me*fh; entradas[2] = md*fh;
      entradas[3] = ob*fv; entradas[4] = oe*fh; entradas[5] = od*fh;
      break;
  }
}

// calcula as entradas para a rede, à partir do estado do jogo, executa a rede
//   com essas entradas e obtém as saídas, converte as saídas da rede em comandos
//   para o jogo.
void processa_rede(Rede rede, estado *j) {
  float entradas[N_ENTRADAS];
  float saidas[N_SAIDAS];
  calcula_entradas(j, entradas);
  calcula_rede(rede, entradas, saidas);
  // as saídas são o quanto a rede acha que tem que ir reto ou pra direita ou esquerda
  if (saidas[1] > saidas[0] && saidas[1] >= saidas[2]) faz_curva(j, pra_direita);
  if (saidas[2] > saidas[0] && saidas[2] >= saidas[1]) faz_curva(j, pra_esquerda);
}

// lê uma tecla, e se for um comando conhecido, reage de acordo
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

// movimenta a cobrinha
// - muda a cabeça para uma nova posição, de acordo com a direção
// - se não tiver aumentando, remove a posição do rabo
// - verifica se bateu em algo e reage de acordo
// - verifica se está há muito tempo sem pegar a maçã, ou se há muito tempo
//   jogando, para garantir que o jogo termina (importante durante o treinamento)
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
  if (!dentro_do_retangulo(pos, j->tela)) {
    j->estado = terminando;
  }
  if (obstaculos_contem(j->obstaculos, pos)) {
    j->estado = terminando;
  }
  if (cobra_contem(j->aninha.corpo, pos)) {
    j->estado = terminando;
  }
  // a nova cabeça passa a fazer parte
  fila_insere(j->aninha.corpo, &pos);
  j->aninha.pos_cabeca = pos;
  // vê se acertou o prêmio
  if (pos.lin == j->maçã.lin && pos.col == j->maçã.col) {
    j->pontos += 317;
    j->aumentando += 5;
    sorteia_premio(j);
    j->passo_ultima_maçã = j->passos;
  }
  // se a cobra não tiver aumentando, tira o último pedaço
  if (j->aumentando == 0) {
    fila_remove(j->aninha.corpo, NULL);
  } else {
    j->aumentando--;
  }
  j->pontos++;
  j->passos++;
  if (j->passos - j->passo_ultima_maçã > 200) {
    j->estado = terminando;
  }
  if (j->max_passos != 0 && j->passos > j->max_passos) {
    j->estado = terminando;
  }
}


int joga_com_tela(Rede rede)
{
  estado jogo;
  inicializa_tela();
  inicializa_jogo(&jogo, 5, 0);

  while (jogo.estado != terminado) {
    desenha_tela(&jogo);
    processa_teclado(&jogo);
    processa_rede(rede, &jogo);
    movimenta(&jogo);
  }

  finaliza_jogo(&jogo);
  finaliza_tela();

  return jogo.pontos;
}

int joga_sem_tela(Rede rede, int tam_ini, int npassos)
{
  estado jogo;
  inicializa_jogo(&jogo, tam_ini, npassos);

  while (jogo.estado != terminando) {
    processa_rede(rede, &jogo);
    movimenta(&jogo);
  }

  finaliza_jogo(&jogo);

  return jogo.pontos;
}

// A função principal
int xmain() {
  return joga_com_tela(NULL);
}
