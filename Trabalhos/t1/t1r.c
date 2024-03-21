// l224a - trabalho 1 - revisão
//
// Aluno:
//
// Você deve alterar este arquivo colocando seu nome na linha acima, e nas linhas 
// delimitadas por comentários indicativos. O restante do arquivo não deve ser
// alterado para implementar a parte I do trabalho.
// Pode adicionar includes também.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum { nenhum, equilatero, isosceles, escaleno, n_tipos } tipo_de_triangulo;
// tipo de dados que representa um triângulo
typedef struct {
  float lado1;
  float lado2;
  float lado3;
  tipo_de_triangulo tipo;
} triangulo;

// tipo de dados que representa um vetor de triângulos
typedef struct {
  int max_triangulos;
  int n_triangulos;                     // número de triângulos no vetor
  triangulo *triangulos; // vetor com os triângulos
} vetor_de_triangulos;

// Você deve implementar as funções faltantes e outras que achar necessário
// abaixo desta linha

// essa função deve existir e ser usada para a leitura de um triângulo.
// lê os valores dos 3 lados de um triângulo,
//   inicializa o triângulo apontado por t com esses valores.
// retorna true se bem sucedido ou false em caso de problema (que pode acontecer na
//   parte II)
bool le_triangulo(triangulo *t, FILE *arq)
{
  float a, b, c;
  //printf("Digite os lados de um triângulo: \n");
  if (fscanf(arq, "%f%f%f", &a, &b, &c) != 3) return false;
  t->lado1 = a;
  t->lado2 = b;
  t->lado3 = c;
  return true;
}

void le_triangulos(vetor_de_triangulos *v, FILE *arq)
{
  for (int i = 0; i < v->n_triangulos; i++) {
    if (!le_triangulo(&v->triangulos[i], arq)) {
      fprintf(stderr, "Erro na leitura do triangulo n. %d\n", i+1);
      exit(1);
    }
  }
  char lixo;
  if (fscanf(arq, " %c", &lixo) == 1) {
    fprintf(stderr, "caracteres extras no final do arquivo\n");
  }
}
tipo_de_triangulo classifica_triangulo(triangulo t)
{
  if (t.lado1 + t.lado2 <= t.lado3) return nenhum;
  if (t.lado2 + t.lado3 <= t.lado1) return nenhum;
  if (t.lado3 + t.lado1 <= t.lado2) return nenhum;
  if (t.lado1 == t.lado2 && t.lado2 == t.lado3) return equilatero;
  if (t.lado1 == t.lado2 || t.lado2 == t.lado3 || t.lado3 == t.lado1) return 2;
  return 3;
}
void classifica_triangulos(vetor_de_triangulos *v)
{
  for (int i = 0; i < v->n_triangulos; i++) {
    v->triangulos[i].tipo = classifica_triangulo(v->triangulos[i]);
  }
}
void conta_triangulos(vetor_de_triangulos *v, int c[])
{
  for (int i = 0; i < v->n_triangulos; i++) {
    tipo_de_triangulo tipo = v->triangulos[i].tipo;
    c[tipo]++;
  }
}
vetor_de_triangulos *aloca_vetor_de_triangulos(int n_triangulos)
{
  vetor_de_triangulos *v;
  v = malloc(sizeof(*v));
  if (v == NULL) return v;
  v->max_triangulos = n_triangulos;
  v->n_triangulos = 0;
  v->triangulos = malloc(n_triangulos * sizeof(triangulo));
  if (v->triangulos == NULL) {
    free(v);
    return NULL;
  }
  return v;
}

// Você não deve alterar abaixo desta linha para a parte I do trabalho

int main()
{
  vetor_de_triangulos *vetor;
  int n_t;
  int contadores[n_tipos] = { 0 };
  FILE *arq;

  arq = fopen("triangulos", "r");
  if (arq == NULL) {
    fprintf(stderr, "erro na abertura do arquivo\n");
    exit(1);
  }

  //printf("Número de triângulos: ");
  if (fscanf(arq, "%d", &n_t) != 1) {
    fprintf(stderr, "erro na leitura do numero de triangulos\n");
    exit(1);
  }
  vetor = aloca_vetor_de_triangulos(n_t);
  if (vetor == NULL) {
    fprintf(stderr, "nao consegui alocar o vetor");
    return 5;
  }
  vetor->n_triangulos = n_t;

  le_triangulos(vetor, arq);
  fclose(arq);
  classifica_triangulos(vetor);
  conta_triangulos(vetor, contadores);

  free(vetor->triangulos);
  free(vetor);

  printf("Classificação dos triângulos:\n");
  printf("  %d equiláteros\n", contadores[equilatero]);
  printf("  %d isósceles\n", contadores[2]);
  printf("  %d escalenos\n", contadores[3]);
  printf("  %d não triângulos\n", contadores[0]);
}
