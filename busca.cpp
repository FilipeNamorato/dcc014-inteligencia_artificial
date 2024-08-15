#include <iostream>
#include <queue> 
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Grafo.h"
#include "busca.h"

//Retorna um iterador para o primeiro elemento no intervalo [first,last)que compara igual a val .
// Se nenhum elemento desse tipo for encontrado, a função retorna last 
#include <algorithm> // para std::find
using namespace std;


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

// Variáveis globais para armazenar as métricas
int nos_expandidos = 0;
int nos_visitados = 0;
int profundidade_solucao = 0;
vector<int> caminho_solucao;

bool encontrar_caminho_bfs(Grafo* grafo, int cidade_origem, int cidade_destino) {
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
