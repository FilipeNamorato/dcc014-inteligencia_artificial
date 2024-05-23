
#include "No.h"
#include "Aresta.h"
using namespace std;

No::No(int idNo){
    this->idNo = idNo;
    this->pesoNo = 0;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::No(int idNo, int peso) {
    this->idNo = idNo;
    this->pesoNo = peso;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::~No() {
    Aresta* auxAresta = getPrimeiraAresta();
    while ( auxAresta != NULL ) {
        Aresta* arestaProx = auxAresta->getProxAresta();
        delete( auxAresta );
        auxAresta = arestaProx;  
    }
}

void No::setIdNo(int valor) {
    this->idNo = valor;
}
int No::getIdNo() {
    return this->idNo;
}

void No::setPeso(int valor) {
    this->pesoNo = valor;
}
int No::getPeso() {
    return this->pesoNo;
}

int No::getGrau() {
    return this->grauSaida;
}
void No::incGrau() {
    this->grauEntrada++;
    this->grauSaida++;
}
void No::decGrau() {
    this->grauEntrada--;
    this->grauSaida--;
}

int No::getGrauEntrada() {
    return this->grauEntrada;
}
void No::incrementaGrauEntrada() {
    this->grauEntrada++;
}
void No::decrementaGrauEntrada() {
    this->grauEntrada--;
}

int No::getGrauSaida() {
    return this->grauSaida;
}
void No::incrementaGrauSaida() {
    this->grauSaida++;
}
void No::decrementaGrauSaida() {
    this->grauSaida--;
}

void No::setProxNo(No *novoNo) {
    this->proxNo = novoNo;
}
No* No::getProxNo(){
    return this->proxNo;
}

void No::setPrimeiraAresta(Aresta *novaAresta) {
    this->primeiraAresta = novaAresta;
}
Aresta* No::getPrimeiraAresta(){
    return this->primeiraAresta;
}