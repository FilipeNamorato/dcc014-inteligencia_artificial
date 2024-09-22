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


void escreveCSV(string nomeMetodo, ofstream &arquivoSaida, int nos_expandidos, int nos_visitados, 
                int profundidade_solucao, float custo_solucao, const vector<int>& caminho_solucao, 
                float tempo_medio){
    
    // Escreve cabeçalho se necessário (opcional, apenas na primeira vez)
    arquivoSaida << "Algoritmo,Nos_Expandidos,Nos_Visitados,Profundidade_Solucao,Custo_Solucao,Caminho_Solucao,Tempo_Medio" << endl;

    // Escreve os dados
    arquivoSaida << nomeMetodo << ","
                 << nos_expandidos << ","
                 << nos_visitados << ","
                 << profundidade_solucao << ","
                 << custo_solucao << ",\"";

    // Escreve o caminho (os valores do vector)
    for (size_t i = 0; i < caminho_solucao.size(); ++i) {
        arquivoSaida << caminho_solucao[i];
        if (i < caminho_solucao.size() - 1) {
            arquivoSaida << " ";  // separador de caminho
        }
    }

    // Fecha aspas do caminho e continua a escrever os outros valores
    arquivoSaida << "\"," << tempo_medio << endl;
}



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

    grafo_cidades->print();
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
    cout << "7 - Todos os métodos" << endl;
    cout << "Sua escolha: ";
    cin >> opcao;

    int numExecs = 100;

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
            caminho_solucao.clear();
            auto inicio = high_resolution_clock::now();
            if (backtracking(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
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

        if (arquivoSaida.is_open()){
            escreveCSV("Backtracking", arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else
            cout << "Erro ao abrir o arquivo." << endl;
         

    } else if (opcao == 2) {
        // Executar a busca em Largura
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaLargura(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, caminho_solucao)) {
                
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
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;
        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

        if (arquivoSaida.is_open()){
            escreveCSV("Busca em Largura",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else
            cout << "Erro ao abrir o arquivo." << endl;
         

    }else if (opcao == 3) {
        // Executar a busca em Profundidade Limitada
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();

            // Corrigido: passando todos os parâmetros esperados para buscaProfundidade
            if (buscaProfundidade(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, custo_acumulado, 0, limite_profundidade, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
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
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;
        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

        if (arquivoSaida.is_open()){
            escreveCSV("Busca em Profundidade Limitada",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else
            cout << "Erro ao abrir o arquivo." << endl;

    } else if (opcao == 4) {
        // Executar a busca Gulosa
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaGulosa(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
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
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;
        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

        if (arquivoSaida.is_open()){
            escreveCSV("Busca Gulosa",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else
            cout << "Erro ao abrir o arquivo." << endl;

    } else if (opcao == 5) {
        // Executar a busca A*
        vector<long long> tempos_execucao;
        for (int i = 0; i < numExecs; ++i) {
            auto inicio = high_resolution_clock::now();
            if (buscaAEstrela(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
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
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;
        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

        if (arquivoSaida.is_open()){
            escreveCSV("A*",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else
            cout << "Erro ao abrir o arquivo." << endl;

    } else if (opcao == 6) {
        // Executar a busca IDA*
        vector<long long> tempos_execucao;
        long long total_nos_expandidos = 0;
        long long total_nos_visitados = 0;
        int maior_profundidade = -1; // Inicializando em -1 para refletir a profundidade correta
        float menor_custo = numeric_limits<float>::infinity(); // Inicializando com infinito

        for (int i = 0; i < numExecs; ++i) {
            // Zerar as variáveis para cada execução
            nos_expandidos = 0;
            nos_visitados = 0;
            profundidade_solucao = -1; // Para indicar que ainda não foi encontrado
            custo_solucao = numeric_limits<float>::infinity(); // Para indicar que não foi encontrado

            auto inicio = high_resolution_clock::now();
            if (buscaIDAEstrela(grafo_cidades, cidade_origem, cidade_destino, 
                    &nos_expandidos, &nos_visitados, &profundidade_solucao, 
                    &custo_solucao, caminho_solucao)) {
                // Atualiza os totais
                total_nos_expandidos += nos_expandidos;
                total_nos_visitados += nos_visitados;

                if (profundidade_solucao > 0) { // Verifica se a profundidade foi alterada
                    maior_profundidade = max(maior_profundidade, profundidade_solucao);
                }

                if (custo_solucao < numeric_limits<float>::infinity()) { // Verifica se o custo foi atualizado
                    menor_custo = min(menor_custo, custo_solucao);
                }
            } else {
                cout << "\nNão existe caminho entre as cidades (usando Busca IDA*)." << endl;
            }
            auto fim = high_resolution_clock::now();
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempos_execucao.push_back(duracao.count());
        }

        // Calcular e imprimir a média dos tempos de execução
        long long tempo_total = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0LL);
        double tempo_medio = static_cast<double>(tempo_total) / numExecs;
        cout << "Tempo médio de execução (IDA*): " << tempo_medio << " microssegundos" << endl;

        ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

        if (arquivoSaida.is_open()) {
            // Escreve os totais acumulados e a média dos tempos
            escreveCSV("IDA*", arquivoSaida, total_nos_expandidos, total_nos_visitados, 
                maior_profundidade, (menor_custo == numeric_limits<float>::infinity() ? 0 : menor_custo), 
                caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

    
    }else if (opcao == 7) {
        // Backtracking
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;
        
        auto inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (backtracking(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
            } else {
                cout << "\nNão existe caminho entre as cidades (Backtracking)." << endl;
            }
        }
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        double tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (Backtracking): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        ofstream arquivoSaida("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("Backtracking",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

        // Busca em Largura
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;

        inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (buscaLargura(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, caminho_solucao)) {
                
            } else {
                cout << "\nNão existe caminho entre as cidades (Largura)." << endl;
            }
        }
        fim = high_resolution_clock::now();
        duracao = duration_cast<microseconds>(fim - inicio);
        tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (Largura): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        arquivoSaida.open("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("Busca em Largura", arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

        // Busca em Profundidade Limitada
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;

        inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (buscaProfundidade(grafo_cidades, cidade_origem, cidade_destino, caminho_solucao, custo_acumulado, 0, limite_profundidade, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
            } else {
                cout << "\nNão existe caminho entre as cidades (Profundidade Limitada)." << endl;
            }
        }
        fim = high_resolution_clock::now();
        duracao = duration_cast<microseconds>(fim - inicio);
        tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (Profundidade Limitada): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        arquivoSaida.open("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("Busca em Profundidade Limitada",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

        // Busca Gulosa
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;

        inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (buscaGulosa(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
            } else {
                cout << "\nNão existe caminho entre as cidades (Gulosa)." << endl;
            }
        }
        fim = high_resolution_clock::now();
        duracao = duration_cast<microseconds>(fim - inicio);
        tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (Gulosa): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        arquivoSaida.open("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("Busca Gulosa" ,arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

        // Busca A*
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;

        inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (buscaAEstrela(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                
            } else {
                cout << "\nNão existe caminho entre as cidades (A*)." << endl;
            }
        }
        fim = high_resolution_clock::now();
        duracao = duration_cast<microseconds>(fim - inicio);
        tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (A*): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        arquivoSaida.open("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("A*",arquivoSaida, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        } else {
            cout << "Erro ao abrir o arquivo." << endl;
        }

        // Busca IDA*
        nos_expandidos = 0;
        nos_visitados = 0;
        profundidade_solucao = 0;
        custo_solucao = 0.0;
        int total_nos_expandidos = 0;
        int total_nos_visitados = 0;
        int total_profundidade_solucao = 0; // ou pode manter o máximo
        float total_custo_solucao = 0.0;

        inicio = high_resolution_clock::now();
        for (int i = 0; i < numExecs; ++i) {
            caminho_solucao.clear();
            if (buscaIDAEstrela(grafo_cidades, cidade_origem, cidade_destino, &nos_expandidos, &nos_visitados, &profundidade_solucao, &custo_solucao, caminho_solucao)) {
                   total_nos_expandidos += nos_expandidos;
                    total_nos_visitados += nos_visitados;
                    total_profundidade_solucao = max(total_profundidade_solucao, profundidade_solucao); // Mantém o máximo
                    total_custo_solucao += custo_solucao;
            } else {
                cout << "\nNão existe caminho entre as cidades (IDA*)." << endl;
            }
        }
        fim = high_resolution_clock::now();
        duracao = duration_cast<microseconds>(fim - inicio);
        tempo_medio = duracao.count() / (double)numExecs;
        cout << "Tempo médio de execução (IDA*): " << tempo_medio << " microssegundos" << endl;

        // Salvar no CSV
        arquivoSaida.open("resultados_busca.csv", ios::app);
        if (arquivoSaida.is_open()) {
            escreveCSV("IDA*",arquivoSaida, total_nos_expandidos, total_nos_visitados, total_profundidade_solucao, total_custo_solucao, caminho_solucao, tempo_medio);
            arquivoSaida.close();
        }else 
            cout << "Erro ao abrir o arquivo." << endl;
        

    } else {
        cout << "Opção inválida!" << endl;
    }

    delete grafo_cidades;

    return 0;
}
