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

    float menor_peso = numeric_limits<float>::infinity();

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        float peso = aresta->getPeso();
        if (peso < menor_peso)
            menor_peso = peso;
    }

    return menor_peso;
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
// funçao recursiva busca em profundidade limitada com custo
float dfs_limitada_com_custo(Grafo* grafo, int cidade_atual, int cidade_destino, 
    std::vector<int>& caminho, float custo_atual, float limite_custo, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao) {

    *nos_visitados += 1;
    caminho.push_back(cidade_atual);

    float f = custo_atual + heuristicaMenorPesoArestaAdjacente(grafo, cidade_atual, cidade_destino); 

    if (cidade_atual == cidade_destino) {
        *profundidade_solucao = caminho.size() - 1;
        caminho_solucao = caminho;
        *custo_solucao = custo_atual;
        cout<<endl<<"Custo atual:"<<custo_atual<<endl;
        return custo_atual; // Solução encontrada
    }

    if (f > limite_custo) {
        caminho.pop_back(); // Restaurar o caminho antes de sair
        return f; // Custo estimado excede o limite, poda o ramo
    }

    No* no_atual = grafo->auxProcurarNoPeloId(cidade_atual);
    if (no_atual == nullptr) {
        caminho.pop_back(); // Restaurar o caminho antes de sair
        return numeric_limits<float>::infinity(); // Cidade não encontrada
    }

    *nos_expandidos += 1; // Atualizar apenas aqui
    float minimo_custo_excedido = numeric_limits<float>::infinity();

    for (Aresta* aresta = no_atual->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        int proxima_cidade = aresta->getNoDestino();
        if (find(caminho.begin(), caminho.end(), proxima_cidade) == caminho.end()) {
            float novo_custo = custo_atual + aresta->getPeso();
            float custo_excedido = dfs_limitada_com_custo(grafo, proxima_cidade, cidade_destino, caminho, 
                novo_custo, limite_custo, nos_expandidos, nos_visitados, profundidade_solucao, 
                custo_solucao, caminho_solucao);

            if (custo_excedido != numeric_limits<float>::infinity()) // Solução encontrada
                return custo_excedido;

            minimo_custo_excedido = min(minimo_custo_excedido, custo_excedido);
        }
    }

    caminho.pop_back(); 
    return minimo_custo_excedido;
}

bool buscaIDAEstrela(Grafo* grafo, int cidade_origem, int cidade_destino, 
    int *nos_expandidos, int *nos_visitados, int *profundidade_solucao, 
    float *custo_solucao, std::vector<int>& caminho_solucao) {

    float limite_custo = heuristicaMenorPesoArestaAdjacente(grafo, cidade_origem, cidade_destino); 

    while (true) {
        vector<int> caminho;
        float custo_excedido = dfs_limitada_com_custo(grafo, cidade_origem, cidade_destino, caminho_solucao, 
            0.0f, limite_custo, nos_expandidos, nos_visitados, profundidade_solucao, custo_solucao, caminho_solucao);

        if (custo_excedido != numeric_limits<float>::infinity()) {
            return true; // Solução encontrada 
        }

        if (custo_excedido == numeric_limits<float>::infinity()) {
            return false; // Não existe solução
        }

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
            for (int i = 0; i < caminho.size(); ++i) {
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
        if (nos_expandidos != 0) {
            fatorRamificacaoMedio = (float)nos_visitados / nos_expandidos;
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
        vector<tuple<int, int, float>> conexoes = {
            {0, 1, 7}, {0, 3, 2}, {1, 2, 3}, {1, 4, 5}, {2, 5, 8},
            {3, 6, 4}, {4, 7, 6}, {5, 8, 2}, {6, 9, 9}, {7, 10, 1},
            {8, 11, 3}, {9, 12, 7}, {10, 13, 5}, {11, 14, 4}, {12, 15, 6},
            {13, 16, 2}, {14, 17, 8}, {15, 18, 3}, {16, 19, 1}, {17, 19, 7}
        };

        for (const auto& conexao : conexoes) {
            int origem, destino;
            float peso;
            tie(origem, destino, peso) = conexao;
            grafo->insertAresta(origem, 0, destino, 0, peso); 
        }

    } else if (tipo == 2) {
      
        int num_nos = 12;

        //variávies para randomizar e possibilitar que crie de forma aleatória as cidades
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < num_nos; ++i) {
            for (int j = i + 1; j < num_nos; ++j) {
                if (dis(gen) < 0.7) { // Probabilidade de 70% de criar uma aresta
                    float peso = 1.0 + dis(gen) * 9.0; // Peso entre 1.0 e 10.0
                    grafo->insertAresta(i, 0, j, 0, peso);
                }
            }
        }

    } else if (tipo == 3) {
        // Grafo esparso 
        int num_nos = 15;
        //variávies para randomizar e possibilitar que crie de forma aleatória as cidades
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < num_nos; ++i) {
            for (int j = i + 1; j < num_nos; ++j) {
                if (dis(gen) < 0.3) { // Probabilidade de 30% de criar uma aresta
                    float peso = 1.0 + dis(gen) * 9.0; // Peso entre 1.0 e 10.0
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