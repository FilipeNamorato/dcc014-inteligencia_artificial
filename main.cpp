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
    // Variáveis passadas por argumento
    //string nomeInstancia = argv[1];
    //int algoritmo = atoi(argv[2]);
    //float alfa = atof(argv[3]);
    //int numIter = atoi(argv[4]);


    // Variáveis globais para armazenar as métricas (declaradas apenas uma vez)
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int profundidade_solucao = 0;
    float custo_solucao = 0.0;
    vector<int> caminho_solucao;


    // Criar o grafo (considerando um grafo não direcionado com pesos nas arestas)
    Grafo* grafo_cidades = new Grafo(false, true, false); 

    // Adicionar as cidades e conexões (exemplo com 8 cidades)
    grafo_cidades->insertAresta(0, 0, 1, 0, 5);   
    grafo_cidades->insertAresta(0, 0, 2, 0, 3);   
    grafo_cidades->insertAresta(1, 0, 2, 0, 2);   
    grafo_cidades->insertAresta(1, 0, 3, 0, 6);   
    grafo_cidades->insertAresta(2, 0, 3, 0, 7);   
    grafo_cidades->insertAresta(2, 0, 4, 0, 4);   
    grafo_cidades->insertAresta(3, 0, 4, 0, 2);  
    grafo_cidades->insertAresta(0, 0, 5, 0, 8);   // Conexão entre cidade 0 e cidade 5
    grafo_cidades->insertAresta(1, 0, 6, 0, 10);  // Conexão entre cidade 1 e cidade 6
    grafo_cidades->insertAresta(3, 0, 5, 0, 3);   // Conexão entre cidade 3 e cidade 5
    grafo_cidades->insertAresta(4, 0, 7, 0, 9);   // Conexão entre cidade 4 e cidade 7
    grafo_cidades->insertAresta(5, 0, 6, 0, 4);   // Conexão entre cidade 5 e cidade 6
    grafo_cidades->insertAresta(5, 0, 7, 0, 7);   // Conexão entre cidade 5 e cidade 7
    grafo_cidades->insertAresta(6, 0, 7, 0, 6);   // Conexão entre cidade 6 e cidade 7

     // Definir as cidades de origem e destino diretamente no código
    int cidade_origem = 0; 
    int cidade_destino = 4; 
    int limite_profundidade = 5; // Defina o limite de profundidade desejado
    vector<int> caminho;
    float custo_solucao_a_estrela;

    int opcao;

    cout << "Escolha uma opção:\n";
    cout << "1 - Busca em Largura\n";
    cout << "2 - Busca em Profundidade Limitada\n";
    cout << "3 - Busca Gulosa\n";
    cout << "4 - Busca A*\n";
    cout << "5 - Busca IDA*\n";
    cout << "Sua escolha: ";
    std::cin >> opcao;

    if (opcao == 1) {
        // Executar a busca em Largura
        auto inicio = high_resolution_clock::now();
        if (buscaLargura(grafo_cidades, cidade_origem, cidade_destino)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca em Largura)." <<endl;

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 2) {
        // Executar a busca em Profundidade Limitada
        auto inicio = high_resolution_clock::now();
        if (buscaProfundidade(grafo_cidades, cidade_origem, cidade_destino, caminho, 0, limite_profundidade)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (ou o limite de profundidade foi atingido)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 3) {
        // Executar a busca Gulosa
        auto inicio = high_resolution_clock::now();
        if (buscaGulosa(grafo_cidades, cidade_origem, cidade_destino)) 
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca Gulosa)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 4) {
        // Executar a busca A*
        auto inicio = high_resolution_clock::now();
        if (buscaAEstrela(grafo_cidades, cidade_origem, cidade_destino, custo_solucao_a_estrela))
            cout << "\nCaminho encontrado" <<endl;
        else 
            cout << "\nNão existe caminho entre as cidades (usando Busca A*)." <<endl;
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        cout << "Tempo de execução: " << duracao.count() << " microssegundos" << endl;

    } else if (opcao == 5) {
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