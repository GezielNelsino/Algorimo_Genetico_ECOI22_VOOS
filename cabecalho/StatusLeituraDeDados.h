#ifndef STATUS_LEITURA_DE_DADOS_H
#define STATUS_LEITURA_DE_DADOS

#include <iostream>

class StatusLeituraDeDados
{
    
public:

    int codigo;

    std::string mensagem;

    StatusLeituraDeDados(int _codigo, std::string _mensagem);

    friend std::ostream &operator<<(std::ostream &s, const StatusLeituraDeDados &status_leitura_de_dados);

    bool eh_codigo_de_erro();

};

#endif