#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Grafo.h"
#include "busca.h"

using namespace std;
using  std::string;
using namespace std::chrono;



int main(int argc, char *argv[]) {
    // Criar o grafo usando a função cria_grafo
    int tipo_grafo = 1; // ou "denso" ou "esparso"
    
    Grafo *grafo_cidades = cria_grafo(1);

    if (grafo_cidades == nullptr) {
        cerr << "Erro ao criar o grafo!" << endl;
        return 1; // Indica um erro na criação do grafo
    }

     // Definir as cidades de origem e destino diretamente no código
    int cidade_origem = 0; 
    int cidade_destino = 4; 
    int limite_profundidade = 5; // Defina o limite de profundidade desejado
    vector<int> caminho;
    float custo_solucao_a_estrela;

    int opcao;

    cout << "Escolha uma opção:\n";
    cout << "1 - Busca em backtraking\n";
    cout << "2 - Busca em Largura\n";
    cout << "3 - Busca em Profundidade Limitada\n";
    cout << "4 - Busca Gulosa\n";
    cout << "5 - Busca A*\n";
    cout << "6 - Busca IDA*\n";
    cout << "Sua escolha: ";
    std::cin >> opcao;

    if (opcao == 1) {
        // Executar o Backtracking
        auto inicio = high_resolution_clock::now();

        if (backtracking(grafo_cidades, cidade_origem, cidade_destino, caminho)) { 
            cout << "\nCaminho encontrado" << endl;
        } else {
            cout << "\nNão existe caminho entre as cidades (usando Backtracking)." << endl;
        }

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    }else if (opcao == 2) {
        // Executar a  em Largura
        auto inicio = high_resolution_clock::now();
        if (buscaLargura(grafo_cidades, cidade_origem, cidade_destino)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca em Largura)." <<endl;

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 3) {
        // Executar a busca em Profundidade Limitada
        auto inicio = high_resolution_clock::now();
        if (buscaProfundidade(grafo_cidades, cidade_origem, cidade_destino, caminho, 0, limite_profundidade)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (ou o limite de profundidade foi atingido)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 4) {
        // Executar a busca Gulosa
        auto inicio = high_resolution_clock::now();
        if (buscaGulosa(grafo_cidades, cidade_origem, cidade_destino)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca Gulosa)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 5) {
        // Executar a busca A*
        auto inicio = high_resolution_clock::now();
        if (buscaAEstrela(grafo_cidades, cidade_origem, cidade_destino, custo_solucao_a_estrela))
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca A*)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 6) {
        // Executar a busca IDA*
        auto inicio = high_resolution_clock::now();
        if(buscaIDAEstrela(grafo_cidades, cidade_origem, cidade_destino))
            cout<< "Caminho econtrado"<<endl;
        else
            cout << "\nNão existe caminho entre as cidades (usando Busca IDA*)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    }else 
        cout << "Opção inválida!" <<endl;
    

    delete grafo_cidades;

    return 0;
}

