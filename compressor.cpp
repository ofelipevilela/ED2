#include "compressor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

Compressor::Compressor(const string& arquivo) : arquivo_entrada(arquivo) {}

double Compressor::calcularTaxaCompressao(size_t tamanho_original, size_t tamanho_comprimido) {
    return (1.0 - static_cast<double>(tamanho_comprimido) / tamanho_original) * 100.0;
}

// Implementação da compressão RLE
double Compressor::compressaoRLE(const string& arquivo_saida) {
    ifstream entrada(arquivo_entrada, ios::binary);
    ofstream saida(arquivo_saida, ios::binary);
    
    if (!entrada || !saida) {
        cerr << "Erro ao abrir arquivos" << endl;
        return 0.0;
    }
    
    size_t tamanho_original = 0;
    char atual, anterior = 0;
    unsigned contador = 0;
    bool primeiro = true;
    
    while (entrada.get(atual)) {
        tamanho_original++;
        
        if (primeiro) {
            anterior = atual;
            contador = 1;
            primeiro = false;
            continue;
        }
        
        if (atual == anterior && contador < 255) {
            contador++;
        } else {
            saida.put(anterior);
            saida.put(contador);
            anterior = atual;
            contador = 1;
        }
    }
    
    // Escreve o último grupo
    if (!primeiro) {
        saida.put(anterior);
        saida.put(contador);
    }
    
    size_t tamanho_comprimido = saida.tellp();  // Usa tellp() para obter o tamanho final
    
    entrada.close();
    saida.close();
    
    return calcularTaxaCompressao(tamanho_original, tamanho_comprimido);
}

// Métodos auxiliares para Huffman
map<char, string> Compressor::gerarCodigosHuffman(shared_ptr<HuffmanNode> raiz, string codigo) {
    map<char, string> codigos;
    
    if (!raiz) return codigos;
    
    if (!raiz->esquerda && !raiz->direita) {
        codigos[raiz->caractere] = codigo;
        return codigos;
    }
    
    auto codigos_esq = gerarCodigosHuffman(raiz->esquerda, codigo + "0");
    auto codigos_dir = gerarCodigosHuffman(raiz->direita, codigo + "1");
    
    codigos.insert(codigos_esq.begin(), codigos_esq.end());
    codigos.insert(codigos_dir.begin(), codigos_dir.end());
    
    return codigos;
}

shared_ptr<HuffmanNode> Compressor::construirArvoreHuffman(const map<char, unsigned>& frequencias) {
    priority_queue<shared_ptr<HuffmanNode>, vector<shared_ptr<HuffmanNode>>, CompararHuffmanNode> fila;
    
    for (const auto& par : frequencias) {
        fila.push(make_shared<HuffmanNode>(par.first, par.second));
    }
    
    while (fila.size() > 1) {
        auto esq = fila.top(); fila.pop();
        auto dir = fila.top(); fila.pop();
        
        auto novo = make_shared<HuffmanNode>('\0', esq->frequencia + dir->frequencia);
        novo->esquerda = esq;
        novo->direita = dir;
        
        fila.push(novo);
    }
    
    return fila.top();
}

void Compressor::salvarCodigosHuffman(const map<char, string>& codigos, ofstream& arquivo) {
    // Salva o número de códigos
    unsigned num_codigos = codigos.size();
    arquivo.write(reinterpret_cast<char*>(&num_codigos), sizeof(num_codigos));
    
    // Salva cada par (caractere, código)
    for (const auto& par : codigos) {
        arquivo.put(par.first);
        unsigned tamanho_codigo = par.second.length();
        arquivo.write(reinterpret_cast<char*>(&tamanho_codigo), sizeof(tamanho_codigo));
        arquivo.write(par.second.c_str(), tamanho_codigo);
    }
}

void Compressor::salvarDadosComprimidos(const string& dados, const map<char, string>& codigos, ofstream& arquivo) {
    string bits;
    for (char c : dados) {
        bits += codigos.at(c);
    }
    
    // Completa com zeros até ter um múltiplo de 8 bits
    while (bits.length() % 8 != 0) {
        bits += "0";
    }
    
    // Converte grupos de 8 bits em bytes e escreve
    for (size_t i = 0; i < bits.length(); i += 8) {
        bitset<8> byte(bits.substr(i, 8));
        arquivo.put(static_cast<char>(byte.to_ulong()));
    }
}

double Compressor::compressaoHuffman(const string& arquivo_saida) {
    // Lê o arquivo e conta frequências
    ifstream entrada(arquivo_entrada, ios::binary);
    if (!entrada) {
        cerr << "Erro ao abrir arquivo de entrada" << endl;
        return 0.0;
    }
    
    map<char, unsigned> frequencias;
    string dados;
    char c;
    size_t tamanho_original = 0;
    
    while (entrada.get(c)) {
        frequencias[c]++;
        dados += c;
        tamanho_original++;
    }
    entrada.close();
    
    // Constrói a árvore de Huffman
    auto raiz = construirArvoreHuffman(frequencias);
    auto codigos = gerarCodigosHuffman(raiz);
    
    // Salva o arquivo comprimido
    ofstream saida(arquivo_saida, ios::binary);
    if (!saida) {
        cerr << "Erro ao criar arquivo de saída" << endl;
        return 0.0;
    }
    
    // Salva a tabela de códigos
    salvarCodigosHuffman(codigos, saida);
    
    // Salva os dados comprimidos
    salvarDadosComprimidos(dados, codigos, saida);
    
    size_t tamanho_comprimido = saida.tellp();
    saida.close();
    
    return calcularTaxaCompressao(tamanho_original, tamanho_comprimido);
} 