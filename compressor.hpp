#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

using namespace std;

// Estrutura para o nó da árvore de Huffman
struct HuffmanNode {
    char caractere;
    unsigned frequencia;
    shared_ptr<HuffmanNode> esquerda, direita;
    
    HuffmanNode(char c, unsigned freq) : caractere(c), frequencia(freq), 
        esquerda(nullptr), direita(nullptr) {}
};

// Comparador para a fila de prioridade
struct CompararHuffmanNode {
    bool operator()(shared_ptr<HuffmanNode> a, shared_ptr<HuffmanNode> b) {
        return a->frequencia > b->frequencia;
    }
};

class Compressor {
private:
    string arquivo_entrada;
    
    // Métodos auxiliares para Huffman
    map<char, string> gerarCodigosHuffman(shared_ptr<HuffmanNode> raiz, string codigo = "");
    shared_ptr<HuffmanNode> construirArvoreHuffman(const map<char, unsigned>& frequencias);
    void salvarCodigosHuffman(const map<char, string>& codigos, ofstream& arquivo);
    void salvarDadosComprimidos(const string& dados, const map<char, string>& codigos, ofstream& arquivo);
    
public:
    Compressor(const string& arquivo);
    
    // Compressão RLE
    double compressaoRLE(const string& arquivo_saida);
    
    // Compressão Huffman
    double compressaoHuffman(const string& arquivo_saida);
    
    // Calcula taxa de compressão
    double calcularTaxaCompressao(size_t tamanho_original, size_t tamanho_comprimido);
};

#endif 