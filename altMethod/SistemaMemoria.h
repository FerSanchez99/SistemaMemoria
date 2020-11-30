#ifndef SISTEMAMEMORIA_H
#define SISTEMAMEMORIA_H

#include "Proceso.h"
#include "MemoriaReal.h"
#include "MemoriaSwap.h"
#include "Stats.h"
#include <unordered_map>

class SistemaMemoria {
    private:
        std::unordered_map<int, Proceso> listaProcesos; //lista que guarda los procesos
        MemoriaReal real;//Objeto clase MemoriaReal que emula una memoria real
        MemoriaSwap swap; //Objeto clase MemoriaSwap que emula una zona de swapping (disco)
        Stats stats;  //Objeto clase Stats que guarda el tiempo de la 'simulaci�n' y otras estad�sticas
    public:
        SistemaMemoria();    //Constructor
        SistemaMemoria(bool);
        Proceso getProceso(int); //regresa el proceso con cierto id
        bool existeProceso(int); //Checa si existe o no cierto proceso
        bool cargarProceso(int, int); //Carga el proceso a memoria principal (opci�n P)
        void accederDirVirtualProceso(int, int, int); //Accede al proceso est� o no en la memoria principal (opci�n A)
        void liberarProceso(int); //Opci�n L
        void limpiarMemorias(); //Borra todo de las memorias
        Stats getStat(); //regresa estad�sticas del sistema de memoria
};

#endif
