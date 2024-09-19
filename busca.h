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

bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);
    
bool buscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);

bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);

// heurística para peso da aresta direta para o destino (se existir)
float heuristicaMenorPesoArestaAdjacente(Grafo* grafo, int cidade_atual, int cidade_destino);

// busca em profundidade limitada com custo (usada na busca IDA*)
float dfs_limitada_com_custo(Grafo* grafo, int cidade_atual, int cidade_destino, 
    std::vector<int>& caminho, float custo_atual, float limite_custo, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao);
//impressão
void imprimirMetricas(string nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo);
void escreverMetricas(const string& nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo);

//auxiliares
Grafo* criaGrafo(int tipo);