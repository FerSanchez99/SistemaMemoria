#include "MemoriaSwap.h"
#include <iostream>

MemoriaSwap::MemoriaSwap() {}

int MemoriaSwap::getTamBytes() {
    return tamBytes;
}

int MemoriaSwap::getPaginasLibres() {
    return paginasLibres;
}

void MemoriaSwap::setPaginasLibres(int paginasLibres) {
    this->paginasLibres = paginasLibres;
}

int MemoriaSwap::asignarPagina(int paginaVirtual, int idProceso) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].second == -1) {
            memoria[i].first = paginaVirtual;
            memoria[i].second = idProceso;
            paginasLibres--;

            return i;
        }
    }

    return -1;
}

void MemoriaSwap::swapIn(Proceso& proceso, MemoriaReal& real, int paginaVirtual, std::unordered_map<int, Proceso>& listaProcesos) {
    int idProceso = proceso.getId();
    int paginaSwap = proceso.getTablaDeMapeo()[paginaVirtual].first;
    memoria[paginaSwap] = std::make_pair(-1, -1);
    paginasLibres++;
    
    if (real.getPaginasLibres() > 0) {
        int paginaReal = real.asignarPagina(paginaVirtual, idProceso);

        proceso.setIndexTablaDeMapeo(paginaVirtual, std::make_pair(paginaReal, true));

        std::cout << "SwapIn - proceso " << idProceso << " de memoria swap en pagina " << paginaSwap 
            << " se movio a memoria real en pagina " << paginaReal << std::endl;
        return;
    }

    paginasLibres--;
    auto paginaProcesoReal = real.intercambiaRealSwap(paginaVirtual, idProceso);
    
    proceso.setIndexTablaDeMapeo(paginaVirtual, std::make_pair(paginaProcesoReal.first, true));
    listaProcesos[paginaProcesoReal.second.second].setIndexTablaDeMapeo(paginaProcesoReal.second.first, std::make_pair(paginaSwap, false));

    std::cout << "Intercambio entre Real y Swap - SwapIn-SwapOut - proceso " << idProceso << " de memoria swap "
            << " en pagina " << paginaSwap << " se movio a memoria real en pagina " << paginaProcesoReal.first
            << " - proceso " << paginaProcesoReal.second.second << " de memoria real en pagina "
            << paginaProcesoReal.first << " se movio a memoria swap en pagina " << paginaSwap << std::endl;
}

void MemoriaSwap::liberarProceso(Proceso proceso) {
    auto tablaProceso = proceso.getTablaDeMapeo();
    int idProceso = proceso.getId();
    
    for (auto dir_enMemoria : tablaProceso) {
        if (!dir_enMemoria.second) {
            paginasLibres++;
            memoria[dir_enMemoria.first] = std::make_pair(-1, -1);

            std::cout << "Se libero proceso " << idProceso << " en memoria swap en pagina " << dir_enMemoria.first << std::endl;
        }
    }
}

void MemoriaSwap::limpiarMemoria() {
    paginasLibres = tamBytes / tamPaginas;
    memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
}