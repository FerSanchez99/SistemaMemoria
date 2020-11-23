#ifndef SISTEMAMEMORIA_H
#define SISTEMAMEMORIA_H

#include "Proceso.h"
#include "MemoriaReal.h"
#include "MemoriaSwap.h"
#include <unordered_map>

class SistemaMemoria {
    private:
        std::unordered_map<int, Proceso> listaProcesos;
        MemoriaReal real;
        MemoriaSwap swap;
    public:
        SistemaMemoria();
        SistemaMemoria(bool);
        Proceso getProceso(int);
        bool existeProceso(int);
        bool cargarProceso(int, int);
        void accederDirVirtualProceso(int, int, int);
        void liberarProceso(int);
        void limpiarMemorias();
};

#endif