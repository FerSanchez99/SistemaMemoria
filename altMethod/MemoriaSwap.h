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
        const int tamBytes = 4096; // Tamano total en bytes de la memoria
        const int tamPaginas = 16; // Tamano de una pagina
        int paginasLibres = tamBytes / tamPaginas; // No. de paginas libres
        // memoria, indice indica pagina, el pair.first pagina virtual del proceso, pair.second idproceso
        std::vector<std::pair<int, int>> memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
    public:
        MemoriaSwap();
        int getTamBytes(); // Regresa el tamno en bytes
        int getPaginasLibres(); // Regresa el no. de paginas libres
        void setPaginasLibres(int); // Set el no. de paginas libres
        int asignarPagina(int, int); // Asigna una sola pagina libre
        void swapIn(Proceso&, MemoriaReal&, int, std::unordered_map<int, Proceso>&, double&, std::pair<int,int>&); // Realiza el swapin
        void liberarProceso(Proceso); // Libera paginas ocupadas por un proceso
        void limpiarMemoria(); // Limpia toda la memoria, lo resetea
};

#endif
