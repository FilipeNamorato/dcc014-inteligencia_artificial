#include <vector>
#include "Grafo.h"

// Declarações das funções de busca
bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino,
    std::vector<int>& caminho, int *nos_expandidos, int *nos_visitados, int *profundidade_solucao,float *custo_solucao, std::vector<int>& caminho_solucao);

bool buscaLargura(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    std::vector<int>& caminho_solucao);

bool buscaProfundidade(Grafo* grafo, int cidade_atual, int cidade_destino, 
    std::vector<int>& caminho, float& custo_acumulado, int profundidade_atual, 
    int limite_profundidade, int *nos_expandidos, int *nos_visitados, 
    int *profundidade_solucao, float *custo_solucao, std::vector<int>& caminho_solucao);

bool buscaOrdenada(Grafo* grafo, int cidade_atual, int cidade_destino,
    std::vector<int>& caminho, float& custo_acumulado, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao,
    float *custo_solucao, std::vector<int>& caminho_solucao);

bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);
    
bool buscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);

bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);

float buscaIDAEstrelaAux(Grafo* grafo, int cidade_atual, int cidade_destino, 
    vector<int>& caminho, int profundidade_atual, float limite, 
    float& custo_acumulado, int *nos_expandidos, int *nos_visitados,
    int *profundidade_solucao, float *custo_solucao, vector<int>& caminho_solucao);

// heurística para peso da aresta direta para o destino (se existir)
float heuristicaMenorPesoArestaAdjacente(Grafo* grafo, int cidade_atual, int cidade_destino);

//impressão
void imprimirMetricas(string nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo, double tempo_medio);

//auxiliares
Grafo* criaGrafo(int tipo);