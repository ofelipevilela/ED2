#define TABELA_HASH

#include "registro.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

registro::registro(const string& nome, unsigned ano, unsigned ocorrencias)
{
    _nome = nome;
    _ano = ano;
    _ocorrencias = ocorrencias;
}

registro::~registro(){
    
}

void registro::set_posicao(size_t posicao) {
    _posicao = posicao;
}

size_t registro::get_posicao() const {
    return _posicao;
}

const string& registro::get_nome() const {
    return _nome;
}

unsigned registro::get_ano() const {
    return _ano;
}

unsigned registro::get_ocorrencias() const {
    return _ocorrencias;
}


