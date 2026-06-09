Algoritmo Genetico para identificação de melhores combinações de voos.

Para compilar, execute:
  g++ main.cpp -o main
Para gerar os resultados, recomendo que salve as saidas em um arquivo chamado saida.txt, isso pode ser feito utilizando o comando:
  ./main > ./saida.txt

Uma combinação de voos é avaliada considerando o custo em dinheiro e o tempo de espera máximo de um indivíduo.

Caso queira alterar os pesos associados ao custo em dinheiro e em tempo, acesse o arquivo cabecalho\state.h e altere as macros.

A branch principal considera apenas os voos de ida. Para executar o algoritmo considerando os voos de volta selecione a branch voos-de-volta.

Caso queira adicinar uma nova cidade isso deve ser feito adicionando sua sigla e nome ao arquivo entrada\cidades.txt, após isso, pelo menos um voo de ida e um voo de volta devem ser adicionados entre essa cidade e roma.

A aplicação considera que todos os voos possuem como origem ou como destino a cidade de roma. Voos que tem horário de saida menor que o horário de entrada não são considerados válidos.