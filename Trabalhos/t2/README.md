## t2 - fila

Os arquivos anexos implementam um jogo em que o jogador deve movimentar uma cobra na tela, evitando obstáculos e tentando acertar prêmios.
O jogo é implementado usando filas.

A implementação de fila fornecida usa alocação contígua (os dados na fila são colocados em uma região de memória alocada na criação da fila) estática (a fila ocupa sempre o mesmo espaço, independentemente do número de dados que contém). Além disso, os dados da fila são mantidos sempre no início da área alocada, sendo realizadas movimentações de dados conforme o conteúdo da fila vai sendo alterado.

Você deve alterar a implementação da fila para:
- evitar a movimentação dos dados, usando "vetor circular";
- evitar a limitação do número de elementos que pode ser colocado na fila, aumentando a quantidade de memória alocada os dados da fila quando houver inserção com o espaço atual todo ocupado.
- evitar o consumo excessivo de memória, diminuindo a quantidade de memória alocada para os dados na fila, quando após uma remoção a fila ficar com excesso de espaço livre.

Para evitar situações extremas, considere uma alocação mínima (uns 10 elementos, por exemplo). Deve sempre ser mantida uma folga quando houver realocação, não deve ser realocado de forma a ficar com o tamanho exato para os dados presentes na fila.
