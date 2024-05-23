#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include "Grafo.h"

// Valor 'infinito'
#define INF 99999

using namespace std;
typedef vector<No*> NodeVector;
// ? EdgeSearchInfo não existe -> Destrutor do No, grafo
// typedef EdgeSearchInfo EdgeInfo;

//------------------------ Construtors and Destrutors ------------------------

/**
 * Grafo Construtor.
 *
 * @param isDigrafo (new valor)
 */
Grafo::Grafo(bool isDigrafo, bool weightArc, bool weightNo) {
    this->ordem = 0;
    this->noRaiz = NULL;
    this->digrafo = isDigrafo;
    this->weigthNo = weightNo;
    this->weightArc = weightArc;

}

/**
 * Destructor
 */
Grafo::~Grafo() {
    No *noAux = getNoRaiz();
    No *noAnterior;
    while ( noAux != NULL ) {
        No *noAuxAnterior = noAux->getProxNo();
        delete( noAux);
        noAux = noAuxAnterior;
    }
}

//----------------------------------- Seters -----------------------------------

/**
 * Funcao incrementa Ordem do grafo em um.
 */
void Grafo::incOrdem() {
    this->ordem++;
}

/**
 * Funcao decrementa Ordem do grafo em um.
 */
void Grafo::decOrdem() {
    this->ordem--;
}

//----------------------------------- Getters -----------------------------------

/**
 * Funcao que retorna se o grafo é ponderado nas arestas.
 */
bool Grafo::getWeightArc() {
    return this->weightArc;
}

/**
 * Funcao que retorna se é ponderado nos nós.
 */
bool Grafo::getWeightNo() {
    return this->weigthNo;
}


//------------------------------ Funcoes do No ------------------------------

/**
 * EauxSSa funcao busca o No pelo nome.
 *
 * @param idFindNo (id do nó)
 * 
 * @return true - se achar no.
 *         false - se não achar.
 */
bool Grafo::procurarNoPeloId(int idFindNo) {
    if(this->auxProcurarNoPeloId(idFindNo)!=NULL){
        return true;
    }
    return false;
}

/**
 * Funcao busca o No pelo nome.
 * 
 *
 * @param idFindNo (id do nó)
 * @return Nó - se achar no.
 *         NULL - se não achar.
 */
No *Grafo::auxProcurarNoPeloId(int idFindNo) {
    No *noAux = this->noRaiz;

    while ( noAux != NULL ) {
        if ( noAux->getIdNo() == idFindNo ) {
            return noAux;
        }
        noAux = noAux->getProxNo();
    }
    return noAux;
}

/**
 * Para achar dois Nós no grafo.
 *
 * @param idFindNo1 (id do nó1)
 * @param idFindNo1 (id do nó2)
 * 
 * @return Nós - se achar.
 *         NULL - se não achar.
 */
retornoNos Grafo::procurarNosPeloId(int idFindNo1, int idFindNo2) {
    No *noAux = this->noRaiz;
    retornoNos nos;
    nos.no1=NULL;
    nos.no2=NULL;

    while ( noAux != NULL ) {
        if ( noAux->getIdNo() == idFindNo1) {
            nos.no1 = noAux;
        }
        else if(noAux->getIdNo() == idFindNo2){
            nos.no2 = noAux;
        }

        if(nos.no1 != NULL  && nos.no2 != NULL){
            return nos;
        }

        noAux = noAux->getProxNo();
    }
    return nos;
}

/**
 * A funcao insere o No caso ele nao existe ou atualiza seu peso.
 *Talvez não vai ser utilizada
 * 
 * @param nome
 * @param pesoNo(peso do nó)
 * 
 * @return nó criado
 */
No *Grafo::insereNo(int idNo, int pesoNo) {
    No *novoNo = auxProcurarNoPeloId(idNo);
    if (novoNo == NULL) {
        novoNo = this->auxInsereNo(idNo, pesoNo);
    } else {
        novoNo->setPeso(pesoNo);
    }
    return novoNo;
}

/**
 * A funcao insere o No sem o verificar se o nó realmente existe.
 * 
 * 
 * @param idNo
 * @param pesoNo (peso do No)
 * 
 * @return nó criado
 */
No *Grafo::auxInsereNo(int idNo, int pesoNo) {
    No *novoNo = (this->weigthNo) ? new No(idNo, pesoNo) : new No(idNo);
    if (this->noRaiz == NULL){
        this->noRaiz = novoNo;
    } else {
        novoNo->setProxNo(this->noRaiz);
        this->noRaiz = novoNo;
    }

    this->incOrdem();
    return novoNo;
}

/**
 * Remove No de um grafo
 *
 * @param idNo (id do nó)
 * @return true - se excluir no´.
 *         false - se não exluir nó
 */
bool Grafo::removeNo(int idNo) {
    // Pesquisa o No a ser excluido
    // Remove todas as arestas/arcos onde este nó ocorre
    // Remove o no

    if(procurarNoPeloId(idNo)){
        No *p = this->noRaiz;
        No *pAnt= NULL;
        int quantArestasRemovido = 0;
        int grauNoRemovido = -1;

        //Lista para quardar os id dos nós que precisam atualizar o grau de entrada
        list<int> listNosAtualiza;

        while (p!=NULL){
            if(p->getIdNo()==idNo){ //Remove todas as arestas do nó a ser removido
                grauNoRemovido =  (isDigraph()) ? p->getGrauSaida() : p->getGrau();

                //Preenche a lista com os ids dos nós que são incididos pelo nó a ser removido
                if(isDigraph()){
                    Aresta *arestaAux = p->getPrimeiraAresta();
                    while(arestaAux!=NULL){
                        listNosAtualiza.push_back(arestaAux->getNoDestino());
                        arestaAux = arestaAux->getProxAresta();
                    }
                }

                No *auxNo = p;
                p = p->getProxNo();
                delete auxNo;
                if(this->noRaiz->getIdNo()==idNo){
                    this->noRaiz=p;
                } else {
                    pAnt->setProxNo(p);
                }
            } else { // Remove as aresta dos nós que incidem incidem no nó a ser removido
                Aresta *arestaRemove = p->getPrimeiraAresta();
                Aresta *arestaAnt = NULL;
                while (arestaRemove!=NULL)
                {
                    if(arestaRemove->getNoDestino()==idNo){
                        Aresta *auxAresta = arestaRemove;
                        (isDigraph()) ? p->decrementaGrauSaida() : p->decGrau();
                        arestaRemove = arestaRemove->getProxAresta();
                        quantArestasRemovido++;
                        
                        if(p->getPrimeiraAresta()->getNoDestino() == idNo){
                            p->setPrimeiraAresta(arestaRemove);
                        } else {
                            arestaAnt->setProxAresta(arestaRemove);
                        }

                        delete auxAresta;
                        
                    } else {
                        arestaAnt = arestaRemove;
                        arestaRemove = arestaRemove->getProxAresta();
                    }
                }

                //Se Lista não estiver vazia, é pra continua atulizando graus de entrada 
                if (!listNosAtualiza.empty()) {
                    int auxTam = listNosAtualiza.size();
                    listNosAtualiza.remove(p->getIdNo());
                    if(auxTam != listNosAtualiza.size()){
                        p->decrementaGrauEntrada();
                    }
                }   
                pAnt=p;
                p = p->getProxNo();
            }
            //Se a quantidade de arestas removidas for igual ao grau do nó a ser removido, então todas arestas que tinha incidem foram removidas 
            if(grauNoRemovido > -1 && quantArestasRemovido == grauNoRemovido && listNosAtualiza.empty()){
                break;
            }
        }

        //Se Lista não estiver vazia, é pra continua atulizando graus de entrada 
        if(!listNosAtualiza.empty()){
            p = this->noRaiz;
            while (p!=NULL){
                if (!listNosAtualiza.empty()) {
                    int auxTam = listNosAtualiza.size();
                    listNosAtualiza.remove(p->getIdNo());
                    if(auxTam != listNosAtualiza.size()){
                        p->decrementaGrauEntrada();
                    }
                } else {
                    break;
                } 
                p = p->getProxNo();
            }
        }

        decOrdem();
        return true;
    } 
    
    return false;
}


/**
 * Insere a aresta de A para B se não adjacencia nao existir,
 * caso exista, a funcao atualiza o peso
 *
 * @param idNoOrigem (id do nó de origem)
 * @param pesoNoOrigem (peso do nó de origem)
 * @param idNoDestino (id do nó destino)
 * @param pesoNoOrigem (peso do nó de destino)
 * @param pesoNoDestino (peso da aresta)
 * 
 */
void Grafo::insertAresta(int idNoOrigem, int pesoNoOrigem, int idNoDestino, int pesoNoDestino, float pesoAresta) {

    No *noOrigem = NULL;
    No *noDestino = NULL;
    
    No *p = this->noRaiz;

    //Busca pelo nós (origem e destino) no grafo, se achar, guarda a referência do nó e atualiza o seu peso
    while (p!=NULL){

        if(p->getIdNo()==idNoOrigem){
            noOrigem = p;
            noOrigem->setPeso(pesoNoOrigem);
        } else if(p->getIdNo()==idNoDestino){
            noDestino =  p;
            noDestino->setPeso(pesoNoDestino);
        }

        if(noOrigem!=NULL && noDestino!=NULL){
            break;
        }
        p = p->getProxNo(); 
    }

    //Se os nós não estiverem no grafo, então é necessário criá-los
    if(noOrigem==NULL){
        noOrigem = this->auxInsereNo(idNoOrigem, pesoNoOrigem);
    }

    if(noDestino==NULL){
        noDestino = this->auxInsereNo(idNoDestino, pesoNoDestino);
    }
    
    //Busca pela aresta NoOrigem, se existir aresta para NoDestino, atualizar o peso da aresta.
    Aresta *aresta = noOrigem->getPrimeiraAresta();
    while (aresta!=NULL){
        if(aresta->getNoDestino()==idNoDestino){
            aresta->setPeso(pesoAresta);
            break;
        }
        aresta = aresta->getProxAresta();
    }

    //Se não existir, então cria a aresta.
    if(aresta == NULL){
        noOrigem->setPrimeiraAresta((this->weightArc) ?  new Aresta(idNoDestino, noOrigem->getPrimeiraAresta(), pesoAresta) : new Aresta(idNoDestino, noOrigem->getPrimeiraAresta()));
        (this->isDigraph()) ? noOrigem->incrementaGrauSaida() : noOrigem->incGrau();
    }
        
    //Busca pela aresta Nodestino, se existir aresta para NoDestino, atualizar o peso da aresta.
    aresta = (this->isDigraph()) ? NULL : noDestino->getPrimeiraAresta();
    while (aresta!=NULL){
        if(aresta->getNoDestino()==idNoOrigem){
            aresta->setPeso(pesoAresta);
            break;
        }
        aresta = aresta->getProxAresta();
    }

    //Se não existir, então cria a aresta (Se for grafo).
    if(aresta == NULL){
        if(this->isDigraph()){
            noDestino->incrementaGrauEntrada();
        } else {
            noDestino->setPrimeiraAresta((this->weightArc) ?  new Aresta(idNoOrigem, noDestino->getPrimeiraAresta(), pesoAresta) : new Aresta(idNoOrigem, noDestino->getPrimeiraAresta()));
            noDestino->incGrau();
        }
    }
}



/**
 * Remove uma aresta entre dois nó
 *
 * @param idNoOrigem (id do nó de origem)
 * @param idNoDestino (id do nó de destino)
 *
 * @return true - se remover aresta.
 *         false - se não achar a aresta a ser removida.
 */
bool Grafo::removeAresta(int idNoOrigem, int idNoDestino) {

    //Busca os nós (Origem e Destino) no grafo
    retornoNos nos = procurarNosPeloId(idNoOrigem, idNoDestino);
    No *noOrigem = nos.no1;
    No *noDestino = nos.no2;

    if(noOrigem != NULL && noDestino != NULL){
        int quantArestasRemovido = 0;

        if(isDigraph()){

            //Buscar e remove a aresta que tem como destino o NóDestino e atualiza o grau de saída do NóOrigem
            Aresta *arestaRemove = noOrigem->getPrimeiraAresta();
            Aresta *arestaAnt = NULL;
            while (arestaRemove!=NULL){
                if(arestaRemove->getNoDestino() == idNoDestino){
                    Aresta *auxAresta = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                    quantArestasRemovido++;
                    noOrigem->decrementaGrauSaida();

                    if(noOrigem->getPrimeiraAresta()->getNoDestino() == idNoDestino){
                        noOrigem->setPrimeiraAresta(arestaRemove);
                    } else {
                        arestaAnt->setProxAresta(arestaRemove);
                    }

                    delete auxAresta;

                } else {
                    arestaAnt = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                }

                if(quantArestasRemovido==1){
                    break;
                }
            }

            //Se realmente aresta existir no NóOrigem, então atualiza o grau de entrada do NóDestino
            if(quantArestasRemovido==1){
                noDestino->decrementaGrauEntrada();
            } 
        } else {

            //Remove a aresta no NóOrigem que tem destino NóDestino
            Aresta *arestaRemove = noOrigem->getPrimeiraAresta();
            Aresta *arestaAnt = NULL;

            while (arestaRemove!=NULL){
                if(arestaRemove->getNoDestino()==idNoDestino){
                    Aresta *auxAresta = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                    quantArestasRemovido++;
                    noOrigem->decGrau();
                    if(noOrigem->getPrimeiraAresta()->getNoDestino() == idNoDestino){
                        noOrigem->setPrimeiraAresta(arestaRemove);
                    } else {
                        arestaAnt->setProxAresta(arestaRemove);
                    }   
                    delete auxAresta;
                } else {
                    arestaAnt = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                }
                if(quantArestasRemovido==1){
                    break;
                }
            }

            //Remove a aresta no NóDestino que tem destino NóOrigem
            arestaRemove = noDestino->getPrimeiraAresta();
            arestaAnt = NULL;
            quantArestasRemovido = 0;

            while (arestaRemove!=NULL){
                if(arestaRemove->getNoDestino()==idNoOrigem){
                    Aresta *auxAresta = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                    quantArestasRemovido++;
                    noDestino->decGrau();
                    if(noDestino->getPrimeiraAresta()->getNoDestino() == idNoOrigem){
                        noDestino->setPrimeiraAresta(arestaRemove);
                    } else {
                        arestaAnt->setProxAresta(arestaRemove);
                    }   
                    delete auxAresta;
                } else {
                    arestaAnt = arestaRemove;
                    arestaRemove = arestaRemove->getProxAresta();
                }
                if(quantArestasRemovido==1){
                    break;
                }
            }

        }

        return quantArestasRemovido;
    }
    return false;
}


//--- Funcoes do Grafo---

/**
 * Imprime o grafo no console
 */
void Grafo::print() {
    No *p = this->noRaiz;
    cout << "-----Grafo-----"<< endl;
    if(this->isDigraph()){
        while (p!=NULL){
            cout<<"Nó: Id: "<<p->getIdNo()<<", Peso: "<<p->getPeso()<<", Grau de entrada: "<<p->getGrauEntrada()<<", Grau de saida: "<<p->getGrauSaida()<<" => Arestas: ";

            Aresta *aresta = p->getPrimeiraAresta();
            while (aresta!=NULL){
                cout<<"IdDestino: "<<aresta->getNoDestino()<<", Peso: "<<aresta->getPeso()<<" | ";
                aresta = aresta->getProxAresta();
            }
            cout << endl;
            p = p->getProxNo(); 
        }
    } else {
        while (p!=NULL){
            cout<<"Nó: Id: "<<p->getIdNo()<<", Peso: "<<p->getPeso()<<", Grau: "<<p->getGrau()<<" => Arestas: ";

            Aresta *aresta = p->getPrimeiraAresta();
            while (aresta!=NULL){
                cout<<"IdDestino: "<<aresta->getNoDestino()<<", Peso: "<<aresta->getPeso()<<" | ";
                aresta = aresta->getProxAresta();
            }
            cout << endl;
            p = p->getProxNo(); 
        }
    }
    cout << "-------------"<< endl;
}

//--- Caracteristica do Grafo ---

/**
 * Retorna o numero de aresta do grafo.
 */
int Grafo::getNumAresta() {
    No *noAux = noRaiz;
    int numArestas = 0;

    while ( noAux != NULL ) {
        numAresta = noAux->getGrau();
        noAux = noAux->getProxNo();
    }
    return numAresta / 2;
}

/**
 * Retorna 'rootNode'.
 */
No *Grafo::getNoRaiz() {
    return this->noRaiz;
}


/**
 * Retorna ordem do grafo.
 */
int Grafo::getOrdem() {
    return this->ordem;
}

/**
 * Retorna grau de entrada.
 */
int Grafo::getGrauEntrada() {
    int inputDegree = 0;

    noAux = noRaiz;
    while ( noAux != NULL ) {
        if ( noAux->getGrauEntrada() > inputDegree ) {
            inputDegree = noAux->getGrauEntrada();
        }
        noAux = noAux->getProxNo();
    }
    return inputDegree;
}

/**
 * Retorna grau de saida.
 */
int Grafo::getGrauSaida() {
    int outputDegree = 0;

    noAux = noRaiz;
    while ( noAux != NULL ) {
        if ( noAux->getGrauSaida() > outputDegree ) {
            outputDegree = noAux->getGrauSaida();
        }
        noAux = noAux->getProxNo();
    }
    return outputDegree;
}

/**
 * Retorna grau do grafo.
 */
void Grafo::getGrau() {
    int inputDegree = 0;
    int outputDegree = 0;

    inputDegree = getGrauEntrada();
    outputDegree = getGrauSaida();

    if ( isDigraph() ) {
        cout << "O grau de ENTRADA do grafo é " << inputDegree << " \ne o de SAIDA e " << outputDegree << ".";
    } else {
        cout << "O GRAU do grafo e " << outputDegree << ".";
    }
}

/**
 * @return true - se grafo e digrafo.
 *        false - se grafo não e digrafo.
 */
bool Grafo::isDigraph() {
    return this->digrafo;
}

Grafo *Grafo::getVerticeInduzido(vector<int> listaNos) {
    No *p = this->noRaiz;
    Grafo *subGrafo = new Grafo(this->isDigraph(), this->getWeightArc(), this->getWeightNo());
    bool *nosSelecionados = new bool[this->getOrdem()];
    noVisitado *nosVisitados = new noVisitado[listaNos.size()];

    // Settando todas posições como false
    for(int i = 0; i < this->getOrdem(); i++) {
        nosSelecionados[i] = false;
    }

    // Marcando quais nós devem ser adicionados ao subgrafo
    for(int i = 0; i < listaNos.size(); i++) {
        nosSelecionados[listaNos[i] - 1] = true;
        nosVisitados[i].visitado = false;
    }

    if(this->isDigraph()) {
        while(p != NULL) {
            if(nosSelecionados[p->getIdNo() - 1]) {
                Aresta *aresta = p->getPrimeiraAresta();
                while(aresta != NULL) {
                    int noAresta = aresta->getNoDestino();
                    if(nosSelecionados[noAresta - 1]) {
                        if(nosVisitados[noAresta - 1].visitado) {
                            subGrafo->insertAresta(p->getIdNo(), p->getPeso(), noAresta, nosVisitados[noAresta - 1].pesoNo, aresta->getPeso());
                        } else {
                            nosVisitados[p->getIdNo() - 1].pesoNo = p->getPeso();
                            nosVisitados[p->getIdNo() - 1].visitado = true;
                        }
                    }
                    aresta = aresta->getProxAresta();
                }
            }
            p = p->getProxNo();
        }
    } else {
        while(p != NULL) {
            if(nosSelecionados[p->getIdNo() - 1]) {
                Aresta *aresta = p->getPrimeiraAresta();
                while(aresta != NULL) {
                    int noAresta = aresta->getNoDestino();
                    if(nosSelecionados[noAresta - 1]) {
                        if(nosVisitados[noAresta - 1].visitado) {
                            subGrafo->insertAresta(p->getIdNo(), p->getPeso(), noAresta, nosVisitados[noAresta - 1].pesoNo, aresta->getPeso());
                        } else {
                            nosVisitados[p->getIdNo() - 1].pesoNo = p->getPeso();
                            nosVisitados[p->getIdNo() - 1].visitado = true;
                        }
                    }
                    aresta = aresta->getProxAresta();
                }
            }
            p = p->getProxNo();
        }
    }
    subGrafo->print();
    return subGrafo;
}

string Grafo::fechoTransitivoDireto(int no){
    bool nosVisitados[ordem];
    string fechoTranDireto = "";

    for(int i = 0; i<ordem; i++)
        nosVisitados[i] = false;

    auxFechoTransitivoDireto(no, nosVisitados);
    nosVisitados[no] = false;

    for(int i = 0; i<ordem; i++){
        
        if(nosVisitados[i]){
            fechoTranDireto += to_string(i) + ", ";
        }
    }

    return fechoTranDireto;
}

void Grafo::auxFechoTransitivoDireto(int idNo, bool *nosVisitados){
    nosVisitados[idNo] = true;
    No *no = auxProcurarNoPeloId(idNo);

    Aresta *aresta = no->getPrimeiraAresta();
    while (aresta!=NULL){    
        //Verifica se o nó está ou não nos visitados
        if(!(nosVisitados[aresta->getNoDestino()])){ 
            auxFechoTransitivoDireto(aresta->getNoDestino(), nosVisitados);
        }
        aresta = aresta->getProxAresta();
    }
}

/**
 * @return Matriz Aresta Peso
 */
vector<vector<float>> Grafo::matrizArestaPeso()
{   
    No *p = this->noRaiz;

    //Inicializa todas posições da matriz com o valor INF
    vector<vector<float>> matriz(this->ordem, vector<float> (this->ordem, INF));

    //Preenche a matriz com o peso de cada aresta que existe no grafo
    while (p!=NULL){
        Aresta *aresta = p->getPrimeiraAresta();
        while (aresta!=NULL){
            matriz[p->getIdNo()][aresta->getNoDestino()] = aresta->getPeso(); 
            aresta = aresta->getProxAresta();
        }
        matriz[p->getIdNo()][p->getIdNo()]=0;
        p = p->getProxNo(); 
    }

    return matriz;
}


/**
 * @return Vetor Nó Peso
 */
vector<int> Grafo::vetNoPeso()
{
    No *p = this->noRaiz;

    //Inicializa vet
    vector<int> vet(this->ordem);

    //Preenche o vetor com o peso de cada nó 
    while (p!=NULL){
        vet[p->getIdNo()]=p->getPeso();
        p = p->getProxNo(); 
    }

    return vet;
}

string Grafo::fechoTransitivoIndireto(int noProcura){
    vector<int> fechoTranInd;
    string fecho = "";
    int ordemGrafo = getOrdem();
    bool nosVisitados[ordemGrafo];
    No * no = getNoRaiz();
    

    for(int i = 0; i<ordemGrafo; i++)
        nosVisitados[i] = false;

    while(no!=NULL){
        auxFechoTransitivoIndireto(no->getIdNo(), noProcura, fechoTranInd, nosVisitados);
        if(nosVisitados[noProcura]){
            
            for(int i = 0; i<fechoTranInd.size();i++){
                fecho += to_string(fechoTranInd[i]) + ", ";
            }
            nosVisitados[noProcura]=false;
        }

        fechoTranInd.clear();
        no = no->getProxNo();
    }

    return fecho;
}

void Grafo::auxFechoTransitivoIndireto(int idNo, int noProcura, vector<int> &fechoTranInd, bool *nosVisitados){
    No *no = auxProcurarNoPeloId(idNo);
    Aresta *aresta = no->getPrimeiraAresta();

    while (aresta!=NULL){

        if(aresta->getNoDestino() != noProcura){ 
            if(nosVisitados[idNo]==false)
                fechoTranInd.push_back(idNo);
            nosVisitados[idNo]=true;
            auxFechoTransitivoIndireto(aresta->getNoDestino(), noProcura, fechoTranInd, nosVisitados);
        }else if(aresta->getNoDestino() == noProcura){
            if(nosVisitados[idNo]==false)
                fechoTranInd.push_back(idNo);
            nosVisitados[idNo]=true;
            nosVisitados[noProcura]=true;
        }
        aresta = aresta->getProxAresta();
    }
    
}