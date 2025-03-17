#ifndef REGISTRO
#define REGISTRO

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

class registro
{
private:
    string      _nome;
    unsigned    _ano;
    unsigned    _ocorrencias;
    size_t      _posicao;

public:
    registro(const string& nome, unsigned ano, unsigned ocorrencias);
    ~registro();

    void set_posicao(size_t posicao);

    const string& get_nome() const;
    unsigned get_ano() const;
    unsigned get_ocorrencias() const;
    size_t get_posicao() const;
};

#endif