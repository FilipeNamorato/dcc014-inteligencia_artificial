#include <iostream>
#include <queue> 
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm> 
#include <limits>

#include "Grafo.h"
#include "busca.h"


using namespace std;
using namespace std::chrono;

// Variáveis globais para armazenar as métricas (declaradas apenas uma vez)
int nos_expandidos = 0;
int nos_visitados = 0;
int profundidade_solucao = 0;
float custo_solucao = 0.0;
vector<int> caminho_solucao;



bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho) {
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        return true; // Caminho encontrado
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {//poteiro nulo
        return false; 
    }

    Aresta* aresta = no_atual->getPrimeiraAresta();
    while (aresta != nullptr) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) { // Verifica se cidade ja visitada
            if (backtracking(grafo, proxima_cidade, cidade_destino, caminho)) {
                return true;
            }
        }
    }
    caminho.pop_back(); // Backtracking
    return false;
}

bool buscaLargura(Grafo* grafo, int cidade_origem, int cidade_destino) {
    vector<bool> visitado(grafo->getOrdem() + 1, false); 
    queue<pair<int, vector<int>>> fila; 
    fila.push({cidade_origem, {cidade_origem}}); 
    visitado[cidade_origem] = true;

    while (!fila.empty()) {
        int cidade_atual = fila.front().first;
        vector<int> caminho_atual = fila.front().second;
        fila.pop();
        nos_visitados++;

        if (cidade_atual == cidade_destino) {
            profundidade_solucao = caminho_atual.size() - 1;
            caminho_solucao = caminho_atual;
            return true; 
        }

        No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
        if (no_atual == nullptr) {
            continue; 
        }

        nos_expandidos++;
        Aresta* aresta = no_atual->getPrimeiraAresta();
        while (aresta != nullptr) {
            int proxima_cidade = aresta->getNoDestino();
            if (!visitado[proxima_cidade]) {
                visitado[proxima_cidade] = true;
                vector<int> novo_caminho = caminho_atual;
                novo_caminho.push_back(proxima_cidade);
                fila.push({proxima_cidade, novo_caminho});
            }
            aresta = aresta->getProxAresta();
        }
    }

    return false; 
}

bool buscaProfundidade(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho, int profundidade_atual, int limite_profundidade) {
    nos_visitados++;
    caminho.push_back(cidade_atual);

    if(cidade_atual == cidade_destino){
        profundidade_solucao = profundidade_atual;
        caminho_solucao = caminho; 

        // Calcula o custo da solução (se necessário)
        // ... (código para calcular o custo, similar ao exemplo da BFS)

        return true; 
    }

    if(profundidade_atual >= limite_profundidade){
        return false; // Limite de profundidade atingido
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if(no_atual == nullptr){
        return false; 
    }

    nos_expandidos++;
    Aresta* aresta = no_atual->getPrimeiraAresta();
    while(aresta != nullptr){
        int proxima_cidade = aresta->getNoDestino();
        if(find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) { 
            if (buscaProfundidade(grafo, proxima_cidade, cidade_destino, caminho, profundidade_atual + 1, limite_profundidade)) {
                return true;
            }
        }
        aresta = aresta->getProxAresta();
    }

    caminho.pop_back(); 
    return false;
}

// Heurística: peso da aresta direta para o destino (se existir)
float heuristicaPesoAresta(Grafo* grafo, int cidade_atual, int cidade_destino) {
    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {
        return numeric_limits<float>::infinity(); // Cidade não encontrada
    }

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        if (aresta->getNoDestino() == cidade_destino) {
            return aresta->getPeso(); // Retorna o peso da aresta direta
        }
    }

    return numeric_limits<float>::infinity(); // Sem aresta direta, retorna um valor alto
}


bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino) {
    vector<bool> visitado(grafo->getOrdem() + 1, false);
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade; 
    filaPrioridade.push({heuristicaPesoAresta(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});
    visitado[cidade_origem] = true;

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first;
        vector<int> caminhoAtual = filaPrioridade.top().second.second;
        filaPrioridade.pop();

        if (cidadeAtual == cidade_destino) {
            cout << "Caminho encontrado: ";
            for (int cidade : caminhoAtual) {
                cout << cidade << " ";
            }
            cout << endl;
            return true;
        }

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr) {
            continue;
        }

        for (Aresta* aresta = noAtual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            int proximaCidade = aresta->getNoDestino();
            if (!visitado[proximaCidade]) {
                visitado[proximaCidade] = true;
                vector<int> novoCaminho = caminhoAtual;
                novoCaminho.push_back(proximaCidade);
                float h = heuristicaPesoAresta(grafo, proximaCidade, cidade_destino);
                filaPrioridade.push({h, {proximaCidade, novoCaminho}});
            }
        }
    }

    return false;
}

// Função para encontrar o caminho usando a busca A* (com cálculo de custos)
bool encontrarCaminhoBuscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, float& custo_solucao) {
    vector<bool> visitado(grafo->getOrdem() + 1, false);
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade;

    filaPrioridade.push({heuristicaPesoAresta(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});
    visitado[cidade_origem] = true;

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first;
        vector<int> caminhoAtual = filaPrioridade.top().second.second;
        float custoAtual = filaPrioridade.top().first - heuristicaPesoAresta(grafo, cidadeAtual, cidade_destino); 
        filaPrioridade.pop();

        if (cidadeAtual == cidade_destino) {
            custo_solucao = custoAtual;
            return true;
        }

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr) {
            continue;
        }

        for (Aresta* aresta = noAtual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            int proximaCidade = aresta->getNoDestino();
            if (!visitado[proximaCidade]) {
                visitado[proximaCidade] = true;
                vector<int> novoCaminho = caminhoAtual;
                novoCaminho.push_back(proximaCidade);
                float novoCusto = custoAtual + aresta->getPeso();
                float f = novoCusto + heuristicaPesoAresta(grafo, proximaCidade, cidade_destino); 
                filaPrioridade.push({f, {proximaCidade, novoCaminho}});
            }
        }
    }

    return false;
}


// Função recursiva para a busca em profundidade limitada com custo
float dfs_limitada_com_custo(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho, float custo_atual, float limite_custo) {
    nos_visitados++;
    caminho.push_back(cidade_atual);

    float f = custo_atual + heuristicaPesoAresta(grafo, cidade_atual, cidade_destino); 

    if (cidade_atual == cidade_destino) {
        profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;
        custo_solucao = custo_atual;
        return custo_atual; // Solução encontrada
    }

    if (f > limite_custo) {
        return f; // Custo estimado excede o limite, poda o ramo
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {
        return numeric_limits<float>::infinity(); // Cidade não encontrada
    }

    nos_expandidos++;
    float minimo_custo_excedido = numeric_limits<float>::infinity();

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            float novo_custo = custo_atual + aresta->getPeso();
            float custo_excedido = dfs_limitada_com_custo(grafo, proxima_cidade, cidade_destino, caminho, novo_custo, limite_custo);

            if (custo_excedido == custo_solucao) { // Solução encontrada em um nível mais profundo
                return custo_solucao;
            }

            minimo_custo_excedido = min(minimo_custo_excedido, custo_excedido);
        }
    }

    caminho.pop_back(); 
    return minimo_custo_excedido;
}

// Função principal da busca IDA*
bool encontrarCaminhoBuscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino) {
    float limite_custo = heuristicaPesoAresta(grafo, cidade_origem, cidade_destino); 

    while (true) {
        nos_expandidos = 0;
        nos_visitados = 0;
        vector<int> caminho;
        float custo_excedido = dfs_limitada_com_custo(grafo, cidade_origem, cidade_destino, caminho, 0.0f, limite_custo);

        if (custo_excedido == custo_solucao) {
            return true; // Solução encontrada
        }

        if (custo_excedido == numeric_limits<float>::infinity()) {
            return false; // Não existe solução
        }

        limite_custo = custo_excedido; 
    }
}
