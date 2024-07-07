## t4 - a maçã

Você deve implementar um TAD grafo, de acordo com `grafo.h`, em um arquivo chamado `grafo.c`.

O restante do código usa um grafo para implementar uma rede neural para jogar o jogo da cobra.
O código da cobra foi alterado para aceitar entrada da rede neural.

Em "compila" tem os comandos para compilar dois programas: `cobra` e `treina`.

O programa `cobra` joga o jogo da cobra, opcionalmente com uma rede neural.
A rede está em um arquivo, passado como primeiro argumento para o programa.
O diretório redes contém algumas redes geradas pelo programa treina.
Por exemplo,
```bash
  ./cobra redes/050-000
```
vai jogar com a rede do arquivo redes/050-000

O programa `treina` gera redes executando um algoritmo de otimização sobre uma rede aleatória.
Para executá-lo, tem que criar um diretório chamado G, onde ele irá gravar uma enormidade de redes.

### FAQ

1. P. Estou em dúvida sobre o funcionamento das funções
   - void grafo_arestas_que_partem(Grafo self, int origem);
   - void grafo_arestas_que_chegam(Grafo self, int destino);
   - bool grafo_proxima_aresta(Grafo self, int *vizinho, void *pdado);

   R. Por exemplo, suponha um grafo com 3 nós (a, b, c) e 3 arestas (ab, ac, bc). Se for pedido arestas_que_partem(a), a primeira chamada a proxima_aresta vai colocar b em vizinho e o dado da aresta ab em pdado, e retornar true; a segunda chamada vai colocar c em vizinho, colocar o dado da aresta ac em pdado e retornar true; a terceira chamada não vai colocar nada nos ponteiros e retornar false. Se for pedido arestar_que_chegam(a), a primeira chamada a proxima_aresta vai retornar false.
