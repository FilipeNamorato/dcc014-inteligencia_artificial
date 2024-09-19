#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Grafo.h"
#include "busca.h"
#include <bits/stdc++.h>

using namespace std;
using  std::string;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    // Verifica se o arquivo existe e remove, se necessário
    if (remove("resultados_busca.csv") == 0);
    int tipo_grafo = 0;

    cout << "Defina o tipo de grafo:" << endl;
    cout << "1 - grande(20 cidades)" << endl;
    cout << "2 - denso(15 cidades)" << endl;
    cout << "3 - esparso (12 cidades)" << endl;

    cin >> tipo_grafo;
    if (tipo_grafo < 1 || tipo_grafo > 3) {
        cout << "Tipo de grafo inválido" << endl;
        return 0;
    }
    Grafo *grafo_cidades = criaGrafo(tipo_grafo);

    if (grafo_cidades == nullptr) {
        cout << "Erro ao criar o grafo!" << endl;
        return 1; // Indica um erro na criação do grafo
    }

    int cidade_origem = 0; 
    int cidade_destino = 4; 
    int limite_profundidade = 5;
    vector<int> caminho;
    float custo_solucao_a_estrela;
    float custo_acumulado;
    int opcao;

    cout << endl << "Escolha uma opção:" << endl;
    cout << "1 - Busca em backtracking" << endl;
    cout << "2 - Busca em Largura" << endl;
    cout << "3 - Busca em Profundidade Limitada" << endl;
    cout << "4 - Busca Gulosa" << endl;
    cout << "5 - Busca A*" << endl;
    cout << "6 - Busca IDA*" << endl;
    cout << "Sua escolha: ";
    cin >> opcao;

    int numExecs = 1;

    // Parâmetros adicionais
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int profundidade_solucao = 0;
    float custo_solucao = 0.0;
    vector<int> caminho_solucao;

    if (opcao == 1) {
        // Executar o Backtracking
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();

            if (backtracking(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                cout << "\nCaminho encontrado" << endl;
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Backtracking)." << endl;
            }

            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "Backtracking," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    } else if (opcao == 2) {
        // Executar a busca em Largura
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaLargura(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, caminho_solucao)) {
                cout << "\nCaminho encontrado" << endl;
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Busca em Largura)." << endl;
            }
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Busca em Largura): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "Largura," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    }else if (opcao == 3) {
        // Executar a busca em Profundidade Limitada
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();

            // Corrigido: passando todos os parâmetros esperados para buscaProfundidade
            if (buscaProfundidade(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, custo_acumulado, 0, limite_profundidade, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                cout << "\nCaminho encontrado" << endl;
                // Você pode exibir ou usar os resultados como necessário
            } else {
                cout << "\nNão existe caminho entre as cidades (ou o limite de profundidade foi atingido)." << endl;
            }

            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Busca em Profundidade Limitada): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "ProfundidadeLimitada," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    } else if (opcao == 4) {
        // Executar a busca Gulosa
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaGulosa(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                cout << "\nCaminho encontrado" << endl;
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Busca Gulosa)." << endl;
            }
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Busca Gulosa): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "Gulosa," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    } else if (opcao == 5) {
        // Executar a busca A*
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaAEstrela(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                cout << "\nCaminho encontrado" << endl;
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Busca A*)." << endl;
            }
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Busca A*): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "AEstrela," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    } else if (opcao == 6) {
        // Executar a busca IDA*
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaIDAEstrela(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                cout << "Caminho encontrado" << endl;
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Busca IDA*)." << endl;
            }
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = (double)tempo_total / numExecs;
        cout << "Tempo médio de execução (Busca IDA*): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append
        if (arquivoSaida.is_open()) {
            arquivoSaida << "IDAEstrela," << tempo_medio << endl;
        } else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
        arquivoSaida.close();

    } else {
        cout << "Opção inválida!" << endl;
    }

    delete grafo_cidades;

    return 0;
}
