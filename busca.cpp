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
#include <random>
#include <tuple>

#include "Grafo.h"
#include "busca.h"


using namespace std;
using namespace chrono;


//====================================================================================================
//                                              backtracking
//====================================================================================================

bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino,
    std::vector<int>& caminho, int *nos_expandidos, int *nos_visitados,
    int *profundidade_solucao, float *custo_solucao, std::vector<int>& caminho_solucao){
    *nos_visitados+=1;
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        *profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;
        return true;
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) // ponteiro nulo
        return false;

    *nos_expandidos+=1;
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            *custo_solucao += aresta->getPeso();  // Adiciona o custo da aresta
            if (backtracking(grafo, proxima_cidade, cidade_destino, caminho, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao)) {
                return true;
            }
            *custo_solucao -= aresta->getPeso();  // Remove o custo da aresta no backtracking
        }
    }

    caminho.pop_back();
    return false;
}
//====================================================================================================
//                                              Busca Largura
//====================================================================================================
bool buscaLargura(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao,
    std::vector<int>& caminho_solucao) {
    vector<bool> visitado(grafo->getOrdem() + 1, false); 
    queue<pair<int, vector<int>>> fila; 
    fila.push({cidade_origem, {cidade_origem}}); 
    visitado[cidade_origem] = true;

    while (!fila.empty()) {
        int cidade_atual = fila.front().first;
        vector<int> caminho_atual = fila.front().second;
        fila.pop();
        *nos_visitados+=1;

        if (cidade_atual == cidade_destino) {
            *profundidade_solucao = caminho_atual.size() - 1;
            caminho_solucao = caminho_atual;
            return true; 
        }

        No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
        if (no_atual == nullptr) continue; 

        *nos_expandidos+=1;
        for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            int proxima_cidade = aresta->getNoDestino();
            if (!visitado[proxima_cidade]) {
                visitado[proxima_cidade] = true;
                vector<int> novo_caminho = caminho_atual;
                novo_caminho.push_back(proxima_cidade);
                fila.push({proxima_cidade, novo_caminho});
            }
        }
    }

    return false; 
}

//====================================================================================================
//                                              Profundidade
//====================================================================================================
bool buscaProfundidade(Grafo* grafo, int cidade_atual, int cidade_destino, 
    std::vector<int>& caminho, float& custo_acumulado, int profundidade_atual, 
    int limite_profundidade, int *nos_expandidos, int *nos_visitados, 
    int *profundidade_solucao, float *custo_solucao, std::vector<int>& caminho_solucao){
    
    *nos_visitados+=1;
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        *profundidade_solucao = profundidade_atual;
        caminho_solucao = caminho;
        *custo_solucao = custo_acumulado;
        return true;
    }

    if (profundidade_atual >= limite_profundidade)
        return false;

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)
        return false;

    *nos_expandidos+=1; 
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        float peso_aresta = aresta->getPeso();
        
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            custo_acumulado += peso_aresta;  // Atualiza o custo acumulado

            if (buscaProfundidade(grafo, proxima_cidade, cidade_destino, caminho, custo_acumulado, profundidade_atual + 1, limite_profundidade, 
                nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao))
                return true;

            custo_acumulado -= peso_aresta;  // Reverte o custo acumulado ao desfazer a recursão
        }
    }

    caminho.pop_back();
    return false;
}


float heuristicaMenorPesoArestaAdjacente(Grafo* grafo, int cidade_atual, int cidade_destino) {
    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr)
        return numeric_limits<float>::infinity(); // Cidade não encontrada

    // Verifica se há uma aresta direta para a cidade destino
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        if (aresta->getNoDestino() == cidade_destino) {
            return aresta->getPeso(); // Retorna o peso da aresta direta
        }
    }

    // Se não houver aresta direta, retorna o menor peso entre as arestas adjacentes
    float menor_peso = numeric_limits<float>::infinity();
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        float peso = aresta->getPeso();
        if (peso < menor_peso)
            menor_peso = peso;
    }

    return menor_peso;
}

//====================================================================================================
//                                              Ordenada
//====================================================================================================
bool buscaOrdenada(Grafo* grafo, int cidade_atual, int cidade_destino,
                   std::vector<int>& caminho, float& custo_acumulado, 
                   int *nos_expandidos, int *nos_visitados, int *profundidade_solucao,
                   float *custo_solucao, std::vector<int>& caminho_solucao) {

    *nos_visitados += 1;
    caminho.push_back(cidade_atual);

    if (cidade_atual == cidade_destino) {
        *profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;
        *custo_solucao = custo_acumulado;
        return true;
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {
        return false;
    }

    *nos_expandidos += 1;

    // Ordenar os vizinhos pela heurística (menor peso da aresta)
    vector<pair<int, float>> vizinhos_ordenados;
    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        float peso_aresta = aresta->getPeso();
        vizinhos_ordenados.push_back({proxima_cidade, peso_aresta});
    }
    sort(vizinhos_ordenados.begin(), vizinhos_ordenados.end(), 
         [](const pair<int, float>& a, const pair<int, float>& b) { 
             return a.second < b.second; 
         });

    for (const auto& vizinho : vizinhos_ordenados) {
        int proxima_cidade = vizinho.first;
        float peso_aresta = vizinho.second;

        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            custo_acumulado += peso_aresta;

            if (buscaOrdenada(grafo, proxima_cidade, cidade_destino, caminho, custo_acumulado, 
                              nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao)) {
                return true;
            }

            custo_acumulado -= peso_aresta;
        }
    }

    caminho.pop_back();
    return false;
}

//====================================================================================================
//                                              Gulosa
//====================================================================================================
bool buscaGulosa(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao){
    
    vector<bool> visitado(grafo->getOrdem() + 1, false);
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade; 
    filaPrioridade.push({heuristicaMenorPesoArestaAdjacente(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});
    visitado[cidade_origem] = true; //vetor bool pra evitar ciclos

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first;
        vector<int> caminhoAtual = filaPrioridade.top().second.second;
        filaPrioridade.pop();
        *nos_visitados+=1; // incrementa nós visitados
        
        if (cidadeAtual == cidade_destino) {
            *profundidade_solucao = caminhoAtual.size() - 1;
            caminho_solucao = caminhoAtual;

            // Calcula o custo da solução 
            No* no_anterior = nullptr;
            for (int cidade : caminhoAtual) {
                No* no_atual = grafo->auxProcurarNoPeloId(cidade);
                if (no_anterior != nullptr) {
                    for (Aresta* aresta = no_anterior->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
                        if (aresta->getNoDestino() == cidade) {
                            *custo_solucao += aresta->getPeso();
                            break;
                        }
                    }
                }
                no_anterior = no_atual;
            }

            return true;
        }

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr)  
            continue;     

        *nos_expandidos+=1; // Incrementa o contador de nós expandidos
        for (Aresta* aresta = noAtual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            int proximaCidade = aresta->getNoDestino();
            if (!visitado[proximaCidade]) {
                visitado[proximaCidade] = true;
                vector<int> novoCaminho = caminhoAtual;
                novoCaminho.push_back(proximaCidade);
                float h = heuristicaMenorPesoArestaAdjacente(grafo, proximaCidade, cidade_destino);
                filaPrioridade.push({h, {proximaCidade, novoCaminho}});
            }
        }
    }

    return false;
}
//====================================================================================================
//                                              A*
//====================================================================================================
// Função para encontrar o caminho usando a busca A*
bool buscaAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao) {
    
    priority_queue<pair<float, pair<int, vector<int>>>, vector<pair<float, pair<int, vector<int>>>>, greater<>> filaPrioridade;
    unordered_map<int, float> custoAtual;
    unordered_map<int, bool> visitado;

    custoAtual[cidade_origem] = 0.0;
    filaPrioridade.push({heuristicaMenorPesoArestaAdjacente(grafo, cidade_origem, cidade_destino), {cidade_origem, {cidade_origem}}});

    while (!filaPrioridade.empty()) {
        int cidadeAtual = filaPrioridade.top().second.first;
        vector<int> caminhoAtual = filaPrioridade.top().second.second;
        float fAtual = filaPrioridade.top().first;
        filaPrioridade.pop();
        *nos_visitados+=1;

        if (cidadeAtual == cidade_destino) {
            *custo_solucao = custoAtual[cidadeAtual];
            *profundidade_solucao = caminhoAtual.size() - 1;
            caminho_solucao = caminhoAtual;

            return true;
        }

        if (visitado[cidadeAtual]) continue;
        visitado[cidadeAtual] = true;
        *nos_expandidos+=1;

        No* noAtual = grafo->auxProcurarNoPeloId(cidadeAtual);
        if (noAtual == nullptr) continue;

        for (Aresta* aresta = noAtual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            int proximaCidade = aresta->getNoDestino();
            float novoCusto = custoAtual[cidadeAtual] + aresta->getPeso();
            
            if (!visitado[proximaCidade] && (custoAtual.find(proximaCidade) == custoAtual.end() || novoCusto < custoAtual[proximaCidade])) {
                custoAtual[proximaCidade] = novoCusto;
                vector<int> novoCaminho = caminhoAtual;
                novoCaminho.push_back(proximaCidade);
                float f = novoCusto + heuristicaMenorPesoArestaAdjacente(grafo, proximaCidade, cidade_destino);
                filaPrioridade.push({f, {proximaCidade, novoCaminho}});
            }
        }
    }

    return false;
}

//====================================================================================================
//                                              IDA*
//====================================================================================================

bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
                     int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
                     float *custo_solucao, std::vector<int>& caminho_solucao) {

    float limite = heuristicaMenorPesoArestaAdjacente(grafo, cidade_origem, cidade_destino); 
    vector<int> caminho = {cidade_origem};
    float custo_acumulado = 0.0;

    while (true) {
        float temp = buscaIDAEstrelaAux(grafo, cidade_origem, cidade_destino, caminho, 0, limite, 
                                        custo_acumulado, nos_expandidos, nos_visitados, profundidade_solucao, 
                                        custo_solucao, caminho_solucao);
        if (temp == -1) { 
            return true; // Solução encontrada
        }
        if (temp == numeric_limits<float>::infinity()) {
            return false; // Não há solução
        }
        limite = temp; 
    }
}

float buscaIDAEstrelaAux(Grafo* grafo, int cidade_atual, int cidade_destino, 
                        vector<int>& caminho, int profundidade_atual, float limite, 
                        float& custo_acumulado, int *nos_expandidos, int *nos_visitados,
                        int *profundidade_solucao, float *custo_solucao, vector<int>& caminho_solucao) {

    *nos_visitados += 1;

    float f = custo_acumulado + heuristicaMenorPesoArestaAdjacente(grafo, cidade_atual, cidade_destino);

    if (f > limite) {
        return f; 
    }

    if (cidade_atual == cidade_destino) {
        *profundidade_solucao = profundidade_atual;
        *custo_solucao = custo_acumulado;
        caminho_solucao = caminho;
        return -1; 
    }

    float min = numeric_limits<float>::infinity();

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {
        return numeric_limits<float>::infinity(); 
    }

    *nos_expandidos += 1;

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) != caminho.end()) {
            continue; 
        }

        caminho.push_back(proxima_cidade);
        custo_acumulado += aresta->getPeso();

        float temp = buscaIDAEstrelaAux(grafo, proxima_cidade, cidade_destino, caminho, profundidade_atual + 1, limite,
                                        custo_acumulado, nos_expandidos, nos_visitados, profundidade_solucao, 
                                        custo_solucao, caminho_solucao);

        if (temp == -1) {
            return -1; 
        }

        if (temp < min) {
            min = temp;
        }

        caminho.pop_back();
        custo_acumulado -= aresta->getPeso();
    }

    return min;
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

    float fatorRamificacaoMedio = 0;
    if (nos_expandidos != 0)
        fatorRamificacaoMedio = (float)nos_visitados / nos_expandidos;
        
    cout << "Fator de ramificação médio: " << fatorRamificacaoMedio << endl;
}

void escreverMetricas(const string& nome_busca, const vector<int>& caminho, int nos_expandidos, int nos_visitados, int profundidade, float custo) {
    ofstream arquivoSaida("resultados_busca.csv", ios::app); // Abre o arquivo em modo append

    if (arquivoSaida.is_open()) {
        // Escreve o cabeçalho apenas na primeira vez que a função é chamada
        static bool primeiraChamada = true;
        if (primeiraChamada) {
            arquivoSaida << "Algoritmo,Caminho,Profundidade,Custo,NosExpandidos,NosVisitados,FatorRamificacaoMedio\n";
            primeiraChamada = false;
        }

        // Escreve os dados da busca no formato CSV
        arquivoSaida << nome_busca << ",";
        
        if (!caminho.empty()) {
            for (size_t i = 0; i < caminho.size(); ++i) {
                arquivoSaida << caminho[i];
                if (i < caminho.size() - 1) {
                    arquivoSaida << "-"; // Separador para as cidades no caminho
                }
            }
            arquivoSaida << "," << profundidade << "," << custo << ",";
        } else {
            arquivoSaida << "null,null,null,"; 
        }

        arquivoSaida << nos_expandidos << "," << nos_visitados << ",";

        float fatorRamificacaoMedio = 0;
        if (nos_expandidos > 0) {
            fatorRamificacaoMedio = static_cast<float>(nos_visitados) / nos_expandidos;
        }
        arquivoSaida << fatorRamificacaoMedio << "\n";

        arquivoSaida.close();
    } else {
        cout << "Erro ao abrir o arquivo de saída!" << endl;
    }
}


//====================================================================================================
//                                          Função gerar tipos de mapas diferentes
//====================================================================================================

Grafo* criaGrafo(int tipo) {
    Grafo* grafo = new Grafo(false, true, false); // Grafo não direcionado com pesos nas arestas

    if (tipo == 1) {
        // Grafo maior com cerca de 20 nós
        vector<tuple<int, int, int>> conexoes = {
            {0, 1, 7}, {0, 3, 2}, {1, 2, 3}, {1, 4, 5}, {2, 5, 8},
            {3, 6, 4}, {4, 7, 6}, {5, 8, 2}, {6, 9, 9}, {7, 10, 1},
            {8, 11, 3}, {9, 12, 7}, {10, 13, 5}, {11, 14, 4}, {12, 15, 6},
            {13, 16, 2}, {14, 17, 8}, {15, 18, 3}, {16, 19, 1}, {17, 19, 7}
        };

        for (const auto& conexao : conexoes) {
            int origem, destino, peso;
            tie(origem, destino, peso) = conexao;
            grafo->insertAresta(origem, 0, destino, 0, peso); 
        }

    } else if (tipo == 2) {
        int num_nos = 12;

        //variáveis para randomizar e possibilitar que crie de forma aleatória as cidades
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10); // Peso entre 1 e 10

        for (int i = 0; i < num_nos; ++i) {
            for (int j = i + 1; j < num_nos; ++j) {
                if (dis(gen) <= 7) { // Probabilidade de 70% de criar uma aresta
                    int peso = dis(gen); // Peso inteiro
                    grafo->insertAresta(i, 0, j, 0, peso);
                }
            }
        }

    } else if (tipo == 3) {
        // Grafo esparso 
        int num_nos = 15;

        //variáveis para randomizar e possibilitar que crie de forma aleatória as cidades
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10); // Peso entre 1 e 10

        for (int i = 0; i < num_nos; ++i) {
            for (int j = i + 1; j < num_nos; ++j) {
                if (dis(gen) <= 3) { // Probabilidade de 30% de criar uma aresta
                    int peso = dis(gen); // Peso inteiro
                    grafo->insertAresta(i, 0, j, 0, peso);
                }
            }
        }

    } else {
        cout << "Tipo de grafo inválido." << endl;
        return nullptr;
    }

    return grafo;
}
