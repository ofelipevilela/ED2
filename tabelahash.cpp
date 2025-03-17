#include "tabelahash.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>


tabelahash::tabelahash(){
    tabela.resize(tamanho);
}

tabelahash::~tabelahash(){
    for (auto& lista : tabela) {
        for (auto* reg : lista) {
            delete reg;
        }
        lista.clear();
    }
}

void tabelahash::ler_csv(const std::string& caminho_csv) {
    std::ifstream arquivo(caminho_csv);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho_csv << std::endl;
        return;
    }

    std::getline(arquivo, linha); // Ignorar cabeçalho

    while (std::getline(arquivo, linha)) {  
        std::stringstream ss(linha);
        std::string id_str, nome, ano_str, genero, count_str;

        std::getline(ss, id_str, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, ano_str, ',');
        std::getline(ss, genero, ',');
        std::getline(ss, count_str, ',');

        try {
            unsigned ano = std::stoi(ano_str);
            unsigned count = std::stoi(count_str);
            insere(nome, ano, count);
        } catch (...) {
            std::cerr << "Erro ao processar linha: " << linha << std::endl;
        }
    }
    
    arquivo.close();
}


int tabelahash::hUm(const string& nome) {
    unsigned long long hashValue = 0;
    const unsigned long long base = 31;
    const unsigned long long mod = 1e9 + 7;

    for (char c : nome) 
        hashValue = (hashValue * base + c) % mod;

    return hashValue % tamanho;
}

int tabelahash::hDois(const string& nome) {
    unsigned long long hashValue = 0;
    const unsigned long long base = 37;
    const unsigned long long mod = 1e9 + 9 - 2;  

    for (char c : nome) 
        hashValue = (hashValue * base + c) % mod;

    return (hashValue % (tamanho - 1)) + 1;
}

void tabelahash::insere(const string& nome, unsigned ano, unsigned count) {
    size_t h1 = hUm(nome);
    size_t h2 = hDois(nome);
    
    for (size_t i = 0; i < tamanho; ++i) {
        size_t h = (h1 + i * h2) % tamanho;
        
        // Se encontrou uma posição vazia ou já tem registros com o mesmo nome
        if (tabela[h].empty() || (!tabela[h].empty() && tabela[h][0]->get_nome() == nome)) {
            tabela[h].push_back(new registro(nome, ano, count));
            return;
        }
    }
    
    std::cerr << "Erro: Não foi possível inserir o registro para " << nome << std::endl;
}

vector<registro*> tabelahash::busca_registros(const string& nome) {
    size_t h1 = hUm(nome);
    size_t h2 = hDois(nome);
    
    for (size_t i = 0; i < tamanho; ++i) {
        size_t h = (h1 + i * h2) % tamanho;
        
        if (tabela[h].empty()) {
            break; // Posição vazia, nome não está na tabela
        }
        
        if (tabela[h][0]->get_nome() == nome) {
            return tabela[h]; // Retorna todos os registros com este nome
        }
    }
    
    return vector<registro*>(); // Retorna vetor vazio se não encontrou
}
