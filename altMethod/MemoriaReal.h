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
        const int tamBytes = 2048; // Tamano total en bytes
        const int tamPaginas = 16; // Tamano de una pagina
        int paginasLibres = tamBytes / tamPaginas; // No. de paginas libres
        bool esFIFO; // Si se esta utilizando una politica FIFO, sino una LRU
        // Lista en la que decide que pagina reemplazar de acuerdo a nuestra politica, pair.first indice de memreal, pair.second idProceso
        std::list<std::pair<int, int>> politicaRemplazoLista; 
        // memoria, indice indica pagina, el pair.first pagina virtual del proceso, pair.second idproceso
        std::vector<std::pair<int, int>> memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
    public:
        MemoriaReal(); // Constructor default
        MemoriaReal(bool); // Constructor default con booleano que indica si es FIFO o no
        bool esPoliticaFIFO(); // Regresa si es FIFO
        void setPoliticaFIFO(bool); // Set politica FIFO o no
        int getTamBytes(); // Regresa el tamano total en bytes
        int getPaginasLibres(); // Regresa el no. de paginas libres
        void setPaginasLibres(int); // Set el no. de paginas libres
        int asignarPagina(int, int); // Asigna una pagina libre, recibe, pagina virtual y id de un proceso.
        // Regresa una pagina de memoria real, (indice de pagina real, (indice de pagina virtual, idProceso))
        std::pair<int, std::pair<int, int>> intercambiaRealSwap(int, int);
        // Asigna un proceso en memoria real
        void asignarProceso(Proceso&, MemoriaSwap&, std::unordered_map<int, Proceso>&, double&,int&);
        void swapOut(MemoriaSwap&, Proceso&, std::pair<int, int>); // Realiza un swapout
        void liberarProceso(Proceso); // Libera un proceso
        void limpiarMemoria(); // Limpiar la memoria, da un reset
        void aplicarLRU(int, int); // Aplica un LRU, mueve la pagina mas recientemente usada
};

#endif
