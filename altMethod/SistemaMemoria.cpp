#include "SistemaMemoria.h"
#include <iostream>

SistemaMemoria::SistemaMemoria() { }

SistemaMemoria::SistemaMemoria(bool esFIFO) {
    real.setPoliticaFIFO(esFIFO);
}

Proceso SistemaMemoria::getProceso(int idProceso) {
    return listaProcesos[idProceso];
}

bool SistemaMemoria::existeProceso(int idProceso) {
    return listaProcesos.find(idProceso) != listaProcesos.end();
}

bool SistemaMemoria::cargarProceso(int idProceso, int tamBytes) {
    Proceso proceso(idProceso, tamBytes);
    if (tamBytes > real.getTamBytes() ||
        (real.getPaginasLibres() + swap.getPaginasLibres()) < tamBytes / proceso.getTamPaginas() + (tamBytes % proceso.getTamPaginas() != 0)) {
        return false;
    }
    
    listaProcesos[idProceso] = proceso;
    real.asignarProceso(listaProcesos[idProceso], swap, listaProcesos);

    return true;
}

void SistemaMemoria::accederDirVirtualProceso(int dirVirtual, int idProceso, int bitModificacion) {
    Proceso proceso = listaProcesos[idProceso];
    int paginaVirtual = dirVirtual / proceso.getTamPaginas();
    auto pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    if (pagProceso.second) {
        if (!real.esPoliticaFIFO()) {
            real.aplicarLRU(pagProceso.first, idProceso);
        }
        std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
                << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
                << " de la Memoria REAL" << std::endl;
        return;
    }

    swap.swapIn(listaProcesos[idProceso], real, paginaVirtual, listaProcesos);

    proceso = listaProcesos[idProceso];
    paginaVirtual = dirVirtual / proceso.getTamPaginas();
    pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
            << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
            << " de la Memoria REAL" << std::endl;
}

void SistemaMemoria::liberarProceso(int idProceso) {
    real.liberarProceso(listaProcesos[idProceso]);
    swap.liberarProceso(listaProcesos[idProceso]);
    listaProcesos.erase(idProceso);
}

void SistemaMemoria::limpiarMemorias() {
    real.limpiarMemoria();
    swap.limpiarMemoria();
    listaProcesos.clear();
}