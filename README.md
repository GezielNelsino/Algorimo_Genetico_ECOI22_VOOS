Algoritmo Genetico para identificação de melhores combinações de voos.

Uma combinação de voos é avaliada considerando o custo em dinheiro e o tempo de espera máximo de um indivíduo.

Para compilar, execute:
  g++ main.cpp -o main
Para gerar os resultados, recomendo que salve as saidas em um arquivo chamado saida.txt, isso pode ser feito utilizando o comando:
  ./main > ./saida.txt

Caso queira alterar os pesos associados ao custo em dinheiro e em tempo, acesse o arquivo state.h e altere as macros.

A branch principal considera apenas os voos de ida. Para executar o algoritmo considerando os voos de volta selecione a branch voos-de-volta
