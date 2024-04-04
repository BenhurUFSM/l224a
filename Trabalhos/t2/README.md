## t2 - fila

Os arquivos anexos implementam um jogo em que o jogador deve movimentar uma cobra na tela, evitando obstáculos e tentando acertar prêmios.
O jogo é implementado usando filas.

A implementação de fila fornecida usa alocação contígua (os dados na fila são colocados em uma região de memória alocada na criação da fila) estática (a fila ocupa sempre o mesmo espaço, independentemente do número de dados que contém). Além disso, os dados da fila são mantidos sempre no início da área alocada, sendo realizadas movimentações de dados conforme o conteúdo da fila vai sendo alterado.

Você deve alterar a implementação da fila para evitar a movimentação dos dados, usando "vetor circular".
Você deve alterar a implementação da fila para evitar o consumo excessivo de memória bem como a limitação do número de elementos que pode ser colocado na fila.
O espaço deve ser realocado conforme a necessidade, aumentando a alocação quando houver inserção com o espaço atual todo ocupado ou diminuindo pela metade a alocação, quando após uma remoção a fila ficar com excesso de espaço livre. Para evitar situações extremas, considere uma alocação mínima (uns 10 elementos, por exemplo). Não aloca sempre o tamanho exato para os elementos existentes.
