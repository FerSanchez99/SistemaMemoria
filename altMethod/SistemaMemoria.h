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
        Stats stats;  //Objeto clase Stats que guarda el tiempo de la 'simulación' y otras estadísticas
    public:
        SistemaMemoria();    //Constructor
        SistemaMemoria(bool);
        Proceso getProceso(int); //regresa el proceso con cierto id
        bool existeProceso(int); //Checa si existe o no cierto proceso
        bool cargarProceso(int, int); //Carga el proceso a memoria principal (opción P)
        void accederDirVirtualProceso(int, int, int); //Accede al proceso esté o no en la memoria principal (opción A)
        void liberarProceso(int); //Opción L
        void limpiarMemorias(); //Borra todo de las memorias
        Stats getStat(); //regresa estadísticas del sistema de memoria
};

#endif
