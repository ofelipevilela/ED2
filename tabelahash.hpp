#ifndef TABELA_HASH
#define TABELA_HASH

#include "registro.hpp"
#include <vector>
#include <string>

using namespace std;


class tabelahash
{
private:
    const size_t tamanho = 2607773; // Número primo próximo a 2.607.761 (tamanho ideal calculado)
    vector<vector<registro*>> tabela; // Agora cada posição é um vector de registros

public:
    tabelahash();
    ~tabelahash();

    void insere(const string& nome, unsigned ano, unsigned ocorrencias);
    int hUm(const string& nome); 
    int hDois(const string& nome);
    void imprime_tabela();
    void maiores_ocorrencias(size_t X);
    void ler_csv(const string& caminho_csv);
    vector<registro*> busca_registros(const string& nome); // Novo método que retorna todos os registros com o nome
};

#endif