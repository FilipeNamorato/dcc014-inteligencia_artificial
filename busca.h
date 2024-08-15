#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Grafo.h"

using namespace std;

class busca {
    public:

        bool backtracking(Grafo* grafo, int cidade_atual, int cidade_destino, vector<int>& caminho);

};