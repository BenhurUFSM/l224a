#include "fila.h"
#include <stdio.h>

int main()
{
    char str[20];
    Fila f = fila_cria(20);
    for (int i=0; i<1; i++) {
        sprintf(str, "%d %d %d", i, 20-i, i);
        fila_insere(f, str);
    }
    fila_inicia_percurso(f, 0);
    while (fila_proximo(f, str)) {
        printf("[%s]\n", str);
    }
    printf("---\n");
    while(!fila_vazia(f)) {
        fila_remove(f, str);
        printf("[%s]\n", str);
    }
    fila_destroi(f);
}
