#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP

#include <string>
#include <vector>
#include "registro.hpp"

using namespace std;

enum class TipoBusca {
    NOME,
    DATA
};

class Buscador {
private:
    string arquivo_csv;
    void desloca(const string& P, vector<int>& d);
    vector<int> computaPrefixo(const string& P);
    registro* criaRegistro(const string& linha);

public:
    Buscador(const string& caminho_csv);
    
    // Busca usando Boyer-Moore-Horspool
    vector<registro*> buscaBMH(const string& padrao, TipoBusca tipo);
    
    // Busca usando KMP
    vector<registro*> buscaKMP(const string& padrao, TipoBusca tipo);
    
    // Limpa memória dos registros retornados
    void limpaRegistros(vector<registro*>& registros);
};

#endif 