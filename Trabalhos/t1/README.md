## t1 - triângulos - revisão de lab I

Esta descrição está incompleta.

Você deve fazer um programa que lê e classifica um grupo de triângulos.

### parte I - sem arquivo nem alocação dinâmica

O programa deve ser feito usando o esqueleto fornecido, sem alterar o código marcado como não sendo alterável.

### parte II - com arquivo

Altere o programa para que os triângulos sejam lidos a partir de um arquivo. Não altere o que é lido, somente de onde. O arquivo deve conter exatamente o que é digitado na parte I.
Altere a função de leitura para receber um `FILE *` que representa o arquivo de onde os dados serão lidos.
O programa deve ler o nome do arquivo no início, e informar ao usuário caso não consiga abrir o arquivo para leitura.

### parte III - com alocação dinâmica

O tipo que representa um vetor de triângulos deve ser alterado para:
```c
typedef struct {
    int max_triangulos;    // número máximo de triângulos que cabe no vetor
    int n_triangulos;      // número de triangulos atualmente no vetor
    triangulo *triangulos; // ponteiro para o primeiro triângulo do vetor
} vetor_de_triangulos;
```
A estrutura acima e a região de memória que conterá os triângulos (apontada pelo campo `triangulos` acima) devem ser alocadas dinamicamente após saber o número de triângulos, usando uma função declarada assim:
```c
vetor_de_triangulos *aloca_vetor_de_triangulos(int n_triangulos);
```
Essa função retorna o vetor alocado (com espaço para o número de triângulos informado mas com 0 triângulos inicializados) ou `NULL` caso a alocação não seja possível. Deve ser corretamente tratado o caso em que uma das alocações é bem sucedida e a outra não.

O restante do programa deve ser alterado para funcionar de acordo com esse novo tipo.
