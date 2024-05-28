## T3 - A árvore

Você deve fazer um programa que mostra um árvore binária de busca, na qual são automaticamente inseridas palavras aleatórias, e removidas palavras digitadas pelo usuário. Você deve seguir a descrição abaixo.

O objetivo do usuário é manter a árvore equilibrada, o programa termina quando a árvore desequilibrar. Ganha-se pontos digitando corretamente palavras existentes na árvore.

A árvore inicia vazia, e o programa sorteia uma palavra por vez, que é inserida na árvore após um tempo aleatório.

A árvore é sempre desenhada na tela, junto com a palavra sorteada e ainda não inserida e a palavra sendo digitada pelo usuário. A palavra do usuário é removida da árvore quando estiver completa (quando o usuário digitar enter).

### Estrutura do programa

O programa tem um laço principal, que executa:
- o processamento de teclas da entrada
- a remoção de palavra digitada da árvore
- a geração de palavra aleatória
- a inserção de palavra aleatória na árvore
- o desenho da árvore

O laço termina quando a árvore ficar desequilibrada ou quando for cancelado pelo usuário.

#### Processamento da entrada

Deve ser mantida uma string com a palavra sendo digitada.
O processamento da entrada lê uma tecla do teclado, e reage da seguinte forma, dependendo do valor da tecla:
- uma letra - insere a letra no final da palavra.
- *backspace* - remove a última letra da palavra, se houver.
- enter - marca a palavra como completa para remoção.
- escape - marca o cancelamento do programa, após pedir confirmação.

Não precisa suportar letras acentuadas nem maiúsculas.

#### remoção de palavra

Caso houver palavra marcada para remoção, remove a palavra da árvore. Se a remoção for bem sucedida, aumenta os pontos do usuário, levando em consideração o número de letras da palavra. Se a remoção não der certo (a palavra não existe na árvore), diminui os pontos.

#### geração de palavra aleatória

Caso não existir uma palavra aleatória sorteada, uma deve ser gerada. 
Para isso, mantenha uma lista de sílabas (de vários tamanhos), sorteie o número de sílabas da palavra e sorteie cada sílaba que ela conterá. Sorteie também um tempo aleatório que a palavra vai demorar até ser inserida na árvore.
Deve haver um limite mínimo e máximo para esse tempo. Esses limites devem ser reduzidos conforme o tempo de execução do programa.

#### inserção da palavra aleatória

Se o tempo de espera para a inserção esgotar, a palavra aleatória deve ser inserida na árvore, e a palavra aleatória marcada como inexistente.

#### desenho da árvore

Para desenhar a árvore, inicialmente deve-se calcular dados de dimensionamento para seus nós, e depois deve ser realizado o desenho, a partir desses dados.

O **cálculo de dimensionamento** deve ser realizado por uma função recursiva, que recebe como argumento um nó da árvore, o nível desse nó na árvore e a posição horizontal onde o desenho dessa árvore deve iniciar. A função deve atualizar os dados de dimensionamento para esse nó e retornar a dimensão horizontal necessária para o desenho da árvore iniciada nesse nó. Um nó vazio não atualiza nada e ocupa espaço zero; um nó não vazio ocupa o espaço de seus filhos mais o seu espaço; a posição da subárvore esquerda é a posição da árvore; a posição da raiz é calculada a partir da posição inicial e da dimensão da subárvore esquerda; a posição da subárvore direita é calculada a partir da posição e da dimensão da raiz.

O **desenho da árvore** deve ser realizado levando em consideração o cálculo anterior. A árvore pode ser desenhada parcialmente, caso não caiba inteira na tela.

Além da árvore, deve aparecer na tela a palavra sendo digitada, a palavra sorteada, o número de pontos.

#### Equilíbrio da árvore

O fator de equilíbrio de um nó é dado pela diferença de altura entre seus filhos, como em uma árvore AVL. O fator de equilíbrio da árvore é o maior fator de equilíbrio entre seus nós, considerando números absolutos (o fator -2 é considerado maior que 1).

A árvore é considerada desequilibrada se seu fator de equilíbrio for muito grande (em números absolutos).

### Outras considerações

O programa deve manter um arquivo de maiores pontuações históricas.

Ao final de uma partida, o programa deve oferecer uma nova; se for aceita, deve realizar
uma nova partida.

Organize seu código. Modularize. Cada função é pequena. Cada função faz uma só coisa. Cada coisa só é feita por uma função.
Faça funções que ajudem a testar, mesmo que não permaneçam no programa final.
Só implemente o que usa a árvore depois de a árvore estar testada.
Só pense em implementar a parte de desenho depois que o resto estiver testado.
