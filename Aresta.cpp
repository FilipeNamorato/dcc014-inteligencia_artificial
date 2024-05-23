#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

using namespace std;

/**
 * Construtor. Os valores padrões são:\n
 * 
 * peso = 0;
 * 
 * @param int noDestino (new valor)
 * @param *proxAresta (new valor)
 */

Aresta::Aresta(int noDestino, Aresta *proxAresta) {
    this->peso = 1;
    this->noDestino = noDestino;
    this->proxAresta = proxAresta;
}

/**
 * Construtor sobrescrito.
 * 
 * @param int noDestino (new valor)
 * @param *proxAresta (new valor)
 * @param peso (new valor)
 */

Aresta::Aresta(int noDestino, Aresta *proxAresta,float pesoAresta) {
    this->peso = pesoAresta;
    this->noDestino = noDestino;
    this->proxAresta = proxAresta;
}


/**
 * A funcao define peso valor.
 * 
 * @param valor (new valor)
 */
void Aresta::setPeso(float valor) {
    this->peso = valor;
}

/**
 * A funcao retorna peso valor.
 */
float Aresta::getPeso() {
    return this->peso;
}

/**
 * A funcao define noDestino.
 * 
 * @param valor (new valor)
 */
void Aresta::setNoDestino(int valor) {
    this->noDestino = valor;
}

/**
 * A funcao retorna noDestino.
 */
int Aresta::getNoDestino() {
    return this->noDestino;
}

/**
 * A funcao define proxAresta.
 * 
 * @param valor (new valor)
 */
void Aresta::setProxAresta(Aresta *valor) {
    this->proxAresta = valor;
}

/**
 * A funcao retorna proxAresta;
 */
Aresta* Aresta::getProxAresta() {
    return this->proxAresta;
}