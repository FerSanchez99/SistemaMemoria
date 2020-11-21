#ifndef MEMORIASWAP_H
#define MEMORIASWAP_H

#include "Proceso.h"
#include "MemoriaReal.h"
#include <list>
#include <utility>
#include <vector>
#include <unordered_map>

class MemoriaReal;

class MemoriaSwap {
    private:
        const int tamBytes = 4096;
        const int tamPaginas = 16;
        int paginasLibres = tamBytes / tamPaginas;
        std::vector<std::pair<int, int>> memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
    public:
        MemoriaSwap();
        int getPaginasLibres();
        void setPaginasLibres(int);
        int asignarPagina(int, int);
        void swapIn(Proceso&, MemoriaReal&, int, std::unordered_map<int, Proceso>&);
        void liberarProceso(Proceso);
};

#endif