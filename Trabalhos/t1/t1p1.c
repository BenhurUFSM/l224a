// l224a - trabalho 1 - revisão
//
// Aluno:
//
// Você deve alterar este arquivo colocando seu nome na linha acima, e nas linhas 
// delimitadas por comentários indicativos. O restante do arquivo não deve ser
// alterado para implementar a parte I do trabalho.

#include <stdio.h>
#include <stdbool.h>

// tipo de dados que representa um triângulo
typedef struct {
  float lado1;
  float lado2;
  float lado3;
  int tipo;
} triangulo;

// número máximo de triângulos suportado pelo programa
#define MAX_TRIANGULOS 100

// tipo de dados que representa um vetor de triângulos
typedef struct {
  int n_triangulos;                     // número de triângulos no vetor
  triangulo triangulos[MAX_TRIANGULOS]; // vetor com os triângulos
} vetor_de_triangulos;

// Você deve implementar as funções faltantes e outras que achar necessário
// abaixo desta linha

// essa função deve existir e ser usada para a leitura de um triângulo.
// lê os valores dos 3 lados de um triângulo,
//   inicializa o triângulo apontado por t com esses valores.
// retorna true se bem sucedido ou false em caso de problema (que pode acontecer na
//   parte II)
bool le_triangulo(triangulo *t)
{
  return true;
}


// Você não deve alterar abaixo desta linha para a parte I do trabalho

int main()
{
  vetor_de_triangulos vetor;
  int n_t;
  int contadores[4] = { 0 };

  printf("Número de triângulos: ");
  scanf("%d", &n_t);
  if (n_t < 1 || n_t > MAX_TRIANGULOS) {
    printf("Deve ser entre %d e %d\n", 1, MAX_TRIANGULOS);
    return 5;
  }
  vetor.n_triangulos = n_t;

  le_triangulos(&vetor);
  classifica_triangulos(&vetor);
  conta_triangulos(&vetor, &contadores);

  printf("Classificação dos triângulos:\n");
  printf("  %d equiláteros\n", contadores[1]);
  printf("  %d isósceles\n", contadores[2]);
  printf("  %d escalenos\n", contadores[3]);
  printf("  %d não triângulos\n", contadores[0]);
}
