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
