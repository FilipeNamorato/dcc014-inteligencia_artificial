#include <vector>
#include "Grafo.h"

// Declarações das funções de busca
bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino, std::vector<int>& caminho);
bool buscaLargura(Grafo* grafo, int cidade_origem, int cidade_destino);
bool buscaProfundidade(Grafo* grafo, int cidade_atual, int cidade_destino, std::vector<int>& caminho, int profundidade_atual, int limite_profundidade);
bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino);
bool buscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino);
bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino);

// heurística para peso da aresta direta para o destino (se existir)
float heuristicaPesoAresta(Grafo* grafo, int cidade_atual, int cidade_destino);

// busca em profundidade limitada com custo (usada na busca IDA*)
float dfs_limitada_com_custo(Grafo* grafo, int cidade_atual, int cidade_destino, std::vector<int>& caminho, float custo_atual, float limite_custo);

//impressão
void imprimirMetricas(string nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo);
void escreverMetricas(const string& nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo);

//auxiliares
Grafo* criaGrafo(int tipo);
