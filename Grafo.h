#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <list>
#include "No.h"
#include "Aresta.h"

using namespace std;

class No;
class Aresta;

//Estrutura para guarda dois nós, utlizada em funções que necessita retorna dois nós
typedef struct {
    No *no1;
    No *no2;
} retornoNos;

typedef struct {
    int pesoNo;
    bool visitado;
} noVisitado;

class Grafo {
public:

    //--- Construtor e Destrutor ---
    Grafo(bool isDigrafo, bool weightArc, bool weightNo);
    ~Grafo();

    //--- Seters ---
    void incOrdem();
    void decOrdem();

    //--- Geters ---
    bool getWeightArc();
    bool getWeightNo();

    //--- Funcoes do No ---
    bool procurarNoPeloId(int idFindNo);
    No *auxProcurarNoPeloId(int idFindNo);
    retornoNos procurarNosPeloId(int idFindNo1, int idFindNo2);
    No *insereNo(int idNo, int pesoNo);
    No *auxInsereNo(int idNo, int pesoNo);
    bool removeNo(int idNo);

    //--- Funcoes de Aresta ---
    void insertAresta(int idNoOrigem, int pesoNoOrigem, int idNoDestino, int pesoNoDestino, float pesoAresta);
    bool removeAresta(int idNoOrigem, int idNoDestino);

    //--- Funcoes do Grafo---
    void print();
    Grafo *getVerticeInduzido(vector<int> listaNos);
    string fechoTransitivoDireto(int idNo);
    void auxFechoTransitivoDireto(int idNo, bool *nosVisitados);
    vector<vector<float>> matrizArestaPeso();
    vector<int> vetNoPeso();
    string fechoTransitivoIndireto(int noProcura);
    void auxFechoTransitivoIndireto(int idNo, int noProcura, vector<int> &fechoTranInd, bool *nosVisitados);
    
    //--- Caracteristica do Grafo ---
    int getNumAresta();
    No *getNoRaiz();
    int getOrdem();
    int getGrauEntrada();
    int getGrauSaida();
    void getGrau();
    bool isDigraph();
    


private:
    
    int ordem;
    int numAresta;
    bool digrafo;
    bool weigthNo;
    bool weightArc;

    No *noRaiz;
    No *noAux;

};


#endif // GRAPH_H