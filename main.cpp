#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Grafo.h"

using namespace std;
using  std::string;
using namespace std::chrono;


int main(int argc, char *argv[]) {
    // Variáveis passadas por argumento
    //string nomeInstancia = argv[1];
    //int algoritmo = atoi(argv[2]);
    //float alfa = atof(argv[3]);
    //int numIter = atoi(argv[4]);

    // Criar o grafo (considerando um grafo não direcionado com pesos nas arestas)
    Grafo* grafo_cidades = new Grafo(false, true, false); 

    // Adicionar as cidades e conexões (exemplo)
    grafo_cidades->insertAresta(0, 0, 1, 0, 10);  // Conexão entre cidade 0 e cidade 1, distância 10
    grafo_cidades->insertAresta(0, 0, 2, 0, 15);  // Conexão entre cidade 0 e cidade 2, distância 15
    grafo_cidades->insertAresta(1, 0, 3, 0, 5);   // Conexão entre cidade 1 e cidade 3, distância 5
    grafo_cidades->insertAresta(2, 0, 3, 0, 8);   // Conexão entre cidade 2 e cidade 3, distância 8

    grafo_cidades->print();
    
    
    // Liberar a memória do grafo
    delete grafo_cidades;

    return 0;
}