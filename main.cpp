#include "tabelahash.hpp"
#include "buscador.hpp"
#include "compressor.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

void imprime_resultados(const vector<registro*>& resultados) {
    if (resultados.empty()) {
        cout << "Nenhum resultado encontrado.\n";
        return;
    }
    
    cout << "\nResultados encontrados:\n";
    for (const auto* reg : resultados) {
        cout << "Nome: " << reg->get_nome() 
             << ", Ano: " << reg->get_ano()
             << ", Ocorrências: " << reg->get_ocorrencias() << "\n";
    }
}

int main() {
    const string arquivo_csv = "Vitor Fernandes Gomes - NationalNames.csv";
    Buscador buscador(arquivo_csv);
    tabelahash tabela;
    bool tabela_hash_criada = false;
    
    while (true) {
        cout << "\nMENU PRINCIPAL:\n";
        cout << "1 - Buscar usando Tabela Hash\n";
        cout << "2 - Buscar usando Boyer-Moore-Horspool\n";
        cout << "3 - Buscar usando Knuth-Morris-Pratt\n";
        cout << "4 - Comprimir usando RLE\n";
        cout << "5 - Comprimir usando Huffman\n";
        cout << "6 - Sair\n";
        cout << "Escolha uma opção: ";
        
        int opcao;
        cin >> opcao;
        
        if (opcao == 6) break;
        
        if (opcao >= 1 && opcao <= 3) {
            string padrao;
            int tipo_busca;
            
            cout << "Escolha o tipo de busca:\n";
            cout << "1 - Buscar por nome\n";
            cout << "2 - Buscar por ano\n";
            cout << "Opção: ";
            cin >> tipo_busca;
            
            cout << "Digite o padrão de busca: ";
            cin >> padrao;
            
            auto inicio = high_resolution_clock::now();
            vector<registro*> resultados;
            
            TipoBusca tipo = (tipo_busca == 1) ? TipoBusca::NOME : TipoBusca::DATA;
            
            switch (opcao) {
                case 1: { // Busca por Hash
                    if (!tabela_hash_criada) {
                        cout << "Criando tabela hash (isso pode demorar um pouco)...\n";
                        tabela.ler_csv(arquivo_csv);
                        tabela_hash_criada = true;
                    }
                    resultados = tabela.busca_registros(padrao);
                    break;
                }
                case 2: // Busca por BMH
                    resultados = buscador.buscaBMH(padrao, tipo);
                    break;
                case 3: // Busca por KMP
                    resultados = buscador.buscaKMP(padrao, tipo);
                    break;
            }
            
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<milliseconds>(fim - inicio);
            
            imprime_resultados(resultados);
            cout << "\nTempo de busca: " << duracao.count() << " ms\n";
            
            if (opcao != 1) { // Limpa memória apenas para BMH e KMP
                buscador.limpaRegistros(resultados);
            }
        }
        else if (opcao == 4 || opcao == 5) {
            Compressor compressor(arquivo_csv);
            auto inicio = high_resolution_clock::now();
            double taxa_compressao;
            
            if (opcao == 4) {
                cout << "\nComprimindo arquivo usando RLE...\n";
                taxa_compressao = compressor.compressaoRLE("saida_rle.txt");
            } else {
                cout << "\nComprimindo arquivo usando Huffman...\n";
                taxa_compressao = compressor.compressaoHuffman("saida_huffman.txt");
            }
            
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<milliseconds>(fim - inicio);
            
            cout << fixed << setprecision(2);
            cout << "\nCompressão concluída!\n";
            cout << "Taxa de compressão: " << taxa_compressao << "%\n";
            cout << "Tempo de compressão: " << duracao.count() << " ms\n";
        }
    }
    
    return 0;
}
