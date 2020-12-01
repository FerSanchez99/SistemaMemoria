#ifndef SISTEMAMEMORIA_H
#define SISTEMAMEMORIA_H

#include "Proceso.h"
#include "MemoriaReal.h"
#include "MemoriaSwap.h"
#include "Stats.h"
#include <unordered_map>

class SistemaMemoria {
    private:
        std::unordered_map<int, Proceso> listaProcesos; // mapa que guarda los procesos que se encuentran en memoria (idProceso -> Proceso)
        MemoriaReal real;// Objeto clase MemoriaReal que emula una memoria real
        MemoriaSwap swap; // Objeto clase MemoriaSwap que emula una zona de swapping (disco)
        Stats stats;  // Objeto clase Stats que guarda el tiempo de la 'simulacion' y otras estad�sticas
    public:
        SistemaMemoria();    // Constructor
        SistemaMemoria(bool); // Constructor con parametro para decidir si es FIFO o no
        Proceso getProceso(int); // regresa el proceso con cierto id
        bool existeProceso(int); // Checa si existe o no cierto proceso
        bool cargarProceso(int, int); // Carga el proceso a memoria principal (opcion P)
        void accederDirVirtualProceso(int, int, int); // Accede al proceso esto o no en la memoria principal (opcion A)
        void liberarProceso(int); // Opcion L
        void limpiarMemorias(); // Borra todo de las memorias
        Stats getStat(); // regresa estadisticas del sistema de memoria
};

#endif
