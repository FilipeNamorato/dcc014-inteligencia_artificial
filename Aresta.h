#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "No.h"

using namespace std;

class No;

class Aresta {
public:
    Aresta(int noDestino, Aresta *proxAresta);
    Aresta(int noDestino, Aresta *proxAresta, float pesoAresta);

    float getPeso();
    void setPeso(float valor);
    int getNoDestino();
    void setNoDestino(int valor);
    Aresta *getProxAresta();
    void setProxAresta(Aresta *valor);
    
    //Aresta *getProxAresta();
    //void setProxAresta(Aresta *valor);
    
private:
    float pesoAresta;
    int noDestino;
    float peso;
    Aresta *proxAresta;
};

#endif // ARESTA_H