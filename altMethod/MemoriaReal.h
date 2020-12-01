#ifndef MEMORIAREAL_H
#define MEMORIAREAL_H

#include "Proceso.h"
#include "MemoriaSwap.h"
#include <list>
#include <utility>
#include <vector>
#include <unordered_map>

class MemoriaSwap;

class MemoriaReal {
    private:
        const int tamBytes = 2048;
        const int tamPaginas = 16;
        int paginasLibres = tamBytes / tamPaginas;
        bool esFIFO;
        std::list<std::pair<int, int>> politicaRemplazoLista;
        std::vector<std::pair<int, int>> memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
    public:
        MemoriaReal();
        MemoriaReal(bool);
        bool esPoliticaFIFO();
        void setPoliticaFIFO(bool);
        int getTamBytes();
        int getPaginasLibres();
        void setPaginasLibres(int);
        int asignarPagina(int, int);
        std::pair<int, std::pair<int, int>> intercambiaRealSwap(int, int);
        void asignarProceso(Proceso&, MemoriaSwap&, std::unordered_map<int, Proceso>&);
        void swapOut(MemoriaSwap&, Proceso&, std::pair<int, int>);
        void liberarProceso(Proceso);
        void limpiarMemoria();
        void aplicarLRU(int, int);
};

#endif