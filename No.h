#ifndef NO_H
#define NO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"


using namespace std;
class Aresta;

class No {

public:
    No(int idNo);
    No(int idNo, int peso);
    
    ~No();

    void setIdNo(int idNo);
    int getIdNo();
    
    void setPeso(int valor);
    int getPeso();

    int getGrau();
    void incGrau();
    void decGrau();

    int getGrauEntrada();
    void incrementaGrauEntrada();

    void decrementaGrauEntrada();

    int getGrauSaida();

    void incrementaGrauSaida();

    void decrementaGrauSaida();

    void setProxNo(No *valor);
    No *getProxNo();

    void setPrimeiraAresta(Aresta *valor);
    Aresta *getPrimeiraAresta();

private:
    int idNo;
    int pesoNo;
    int grauEntrada;
    int grauSaida;
    No *proxNo;
    Aresta *primeiraAresta;
};

#endif // NO_H