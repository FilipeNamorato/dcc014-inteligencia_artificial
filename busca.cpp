#include <iostream>
#include <queue> 
#include <string>
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

//====================================================================================================
//                                              backtracking
//====================================================================================================

bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho) {
    nos_visitados++;
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;

        // Calcula o custo da solução 
        No* no_anterior = nullptr;
        for (int cidade : caminho) {
            No* no_atual = grafo->auxProcurarNoPeloId(cidade);
            if (no_anterior != nullptr) {
                for (Aresta* aresta = no_anterior->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
                    if (aresta->getNoDestino() == cidade) {
                        custo_solucao += aresta->getPeso();
                        break;
                    }
                }
            }
            no_anterior = no_atual;
        }
        
        return true;
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)//ponteiro nulo
        return false;

    nos_expandidos++;
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            if (backtracking(grafo, proxima_cidade, cidade_destino, caminho))
                return true;
        }
    }

    caminho.pop_back();
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
        if (no_atual == nullptr)
            continue; 

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
//====================================================================================================
//                                              Profundidade
//====================================================================================================
bool buscaProfundidade(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho, int profundidade_atual, int limite_profundidade) {
    nos_visitados++;
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        profundidade_solucao = profundidade_atual;
        caminho_solucao = caminho;

        // Calcula o custo da solução
        No* no_anterior = nullptr;
        for (int cidade : caminho) {
            No* no_atual = grafo->auxProcurarNoPeloId(cidade);
            if (no_anterior != nullptr) {
                for (Aresta* aresta = no_anterior->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
                    if (aresta->getNoDestino() == cidade) {
                        custo_solucao += aresta->getPeso();
                        break;
                    }
                }
            }
            no_anterior = no_atual;
        }

        return true;
    }

    if (profundidade_atual >= limite_profundidade)
        return false; // Limite de profundidade atingido

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)
        return false;

    nos_expandidos++; // Incrementa o contador de nós expandidos
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            if (buscaProfundidade(grafo, proxima_cidade, cidade_destino, caminho, profundidade_atual + 1, limite_profundidade))
                return true;
        }
    }

    caminho.pop_back();
    return false;
}

// Heurística: peso da aresta direta para o destino (se existir)
float heuristicaPesoAresta(Grafo* grafo, int cidade_atual, int cidade_destino) {
    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)
        return numeric_limits<float>::infinity(); // Cidade não encontrada

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        if (aresta->getNoDestino() == cidade_destino) 
            return aresta->getPeso(); // Retorna o peso da aresta direta
    }

    return numeric_limits<float>::infinity(); // Sem aresta direta, retorna um valor alto
}


//====================================================================================================
//                                              Gulosa
//====================================================================================================
bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino) {
    vector<bool> visitado(grafo->getOrdem() + 1, false);
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade; 
    filaPrioridade.push({heuristicaPesoAresta(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});
    visitado[cidade_origem] = true;

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first; //VERIFICAR
        vector<int> caminhoAtual = filaPrioridade.top().second.second; //VERIFICAR
        filaPrioridade.pop();

        if (cidadeAtual == cidade_destino) {
            cout << "Caminho encontrado: ";
            for (int cidade : caminhoAtual)
                cout << cidade << " ";
            cout << endl;
            return true;
        }

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr)  //ponterio nulo
            continue;     

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
//====================================================================================================
//                                              A*
//====================================================================================================
// Função para encontrar o caminho usando a busca A* (com cálculo de custos e métricas)
bool buscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, float& custo_solucao) {
    // Reinicializar as métricas para esta busca
    nos_expandidos = 0;
    nos_visitados = 0;
    profundidade_solucao = 0;
    caminho_solucao.clear();

    vector<bool> visitado(grafo->getOrdem() + 1, false);
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade;

    filaPrioridade.push({heuristicaPesoAresta(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});
    visitado[cidade_origem] = true;

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first;
        vector<int> caminhoAtual = filaPrioridade.top().second.second;
        float custoAtual = filaPrioridade.top().first - heuristicaPesoAresta(grafo, cidadeAtual, cidade_destino);
        filaPrioridade.pop();
        nos_visitados++; //nós visit

        if (cidadeAtual == cidade_destino) {
            custo_solucao = custoAtual;
            profundidade_solucao = caminhoAtual.size() - 1;
            caminho_solucao = caminhoAtual;
            return true;
        }

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr)
            continue;

        nos_expandidos++; //nós expan
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
//====================================================================================================
//                                              IDA*
//====================================================================================================
// funçao recursiva busca em profundidade limitada com custo
float dfs_limitada_com_custo(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho, float custo_atual, float limite_custo) {
    nos_visitados++;
    caminho.push_back(cidade_atual);

    float f = custo_atual + heuristicaPesoAresta(grafo, cidade_atual, cidade_destino); 

    if (cidade_atual == cidade_destino) {
        profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;
        custo_solucao = custo_atual;
        return custo_atual; // soluçao encontrada
    }

    if (f > limite_custo) 
        return f; // custo estimado excede o limite, poda o ramo

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)
        return numeric_limits<float>::infinity(); // cidade n encontrada

    nos_expandidos++;
    float minimo_custo_excedido = numeric_limits<float>::infinity();

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            float novo_custo = custo_atual + aresta->getPeso();
            float custo_excedido = dfs_limitada_com_custo(grafo, proxima_cidade, cidade_destino, caminho, novo_custo, limite_custo);

            if (custo_excedido == custo_solucao) // soluçao encontrada em um nivel mais profundo
                return custo_solucao;

            minimo_custo_excedido = min(minimo_custo_excedido, custo_excedido);
        }
    }

    caminho.pop_back(); 
    return minimo_custo_excedido;
}

// Função principal busca IDA*
bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino) {
    float limite_custo = heuristicaPesoAresta(grafo, cidade_origem, cidade_destino); 

    while (true) {
        nos_expandidos = 0;
        nos_visitados = 0;
        vector<int> caminho;
        float custo_excedido = dfs_limitada_com_custo(grafo, cidade_origem, cidade_destino, caminho, 0.0f, limite_custo);

        if (custo_excedido == custo_solucao) 
            imprimirMetricas("IDA*",caminho,nos_expandidos,nos_visitados, profundidade_solucao, custo_solucao);
            return true; // soluçao encontrada 

        if (custo_excedido == numeric_limits<float>::infinity())
            imprimirMetricas("IDA*",caminho,nos_expandidos,nos_visitados, profundidade_solucao, custo_solucao); 
            return false; // nao existe solução

        limite_custo = custo_excedido; 
    }
}

//====================================================================================================
//                                          impressão métricas
//====================================================================================================

void imprimirMetricas(string nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo) {
    cout << "\nResultados da " << nome_busca << ":" << endl;
    if (!caminho.empty()) {
        cout << "Caminho encontrado: ";
        for(int cidade : caminho) 
            cout << cidade << " ";
        cout << endl;
        cout << "Profundidade da solução: " << profundidade << endl;
        cout << "Custo da solução: " << custo << endl;
    
    }else 
        cout << "Não existe caminho entre as cidades." << endl;

    cout << "Nós expandidos: " << nos_expandidos << endl;
    cout << "Nós visitados: " << nos_visitados << endl;

    float fator_ramificacao_medio = 0;
    if (nos_expandidos != 0)
        fator_ramificacao_medio = (float)nos_visitados / nos_expandidos;
        
    cout << "Fator de ramificação médio: " << fator_ramificacao_medio << endl;
}