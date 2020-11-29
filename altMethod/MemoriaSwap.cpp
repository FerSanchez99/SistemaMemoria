#include "MemoriaSwap.h"
#include <utility>
#include <iostream>
using namespace std;
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

void MemoriaSwap::swapIn(Proceso& proceso, MemoriaReal& real, int paginaVirtual, std::unordered_map<int, Proceso>& listaProcesos, double &dTime, pair<int,int>& swaps) {
    int idProceso = proceso.getId();
    int paginaSwap = proceso.getTablaDeMapeo()[paginaVirtual].first;
    memoria[paginaSwap] = std::make_pair(-1, -1);
    paginasLibres++;

    //existe espacio para hacer swap in de todo el proceso
    if (real.getPaginasLibres() > 0) {
        int paginaReal = real.asignarPagina(paginaVirtual, idProceso);

        proceso.setIndexTablaDeMapeo(paginaVirtual, std::make_pair(paginaReal, true));

        std::cout << "SwapIn - PROCESO" << idProceso << " de Memoria SWAP en pagina " << paginaSwap
            << " se movio a Memoria REAL en pagina " << paginaReal << std::endl;
        dTime+=0.1;
        return;
        swaps.first++;
    }

    //primero es necesario un swap out
    paginasLibres--;
    auto paginaProcesoReal = real.intercambiaRealSwap(paginaVirtual, idProceso);

    proceso.setIndexTablaDeMapeo(paginaVirtual, std::make_pair(paginaProcesoReal.first, true));
    listaProcesos[paginaProcesoReal.second.second].setIndexTablaDeMapeo(paginaProcesoReal.second.first, std::make_pair(paginaSwap, false));

    std::cout << "Intercambio entre REAL y SWAP - SwapIn-SwapOut:" << std::endl
            << "SwapIn - PROCESO " << idProceso << " de Memoria SWAP "
            << " en pagina " << paginaSwap << " se movio a Memoria REAL en pagina " << paginaProcesoReal.first << std::endl
            << "SwapOut - PROCESO " << paginaProcesoReal.second.second << " de Memoria REAL en pagina "
            << paginaProcesoReal.first << " se movio a Memoria SWAP en pagina " << paginaSwap << std::endl;
            dTime+=2; //1 seg por el swap out y un 1 seg por el swap in
            swaps.first++;
            swaps.second++;

}

void MemoriaSwap::liberarProceso(Proceso proceso) {
    auto tablaProceso = proceso.getTablaDeMapeo();
    int idProceso = proceso.getId();

    for (auto dir_enMemoria : tablaProceso) {
        if (!dir_enMemoria.second) {
            paginasLibres++;
            memoria[dir_enMemoria.first] = std::make_pair(-1, -1);

            std::cout << "Se libero PROCESO " << idProceso << " en Memoria SWAP en pagina " << dir_enMemoria.first << std::endl;
        }
    }
}

void MemoriaSwap::limpiarMemoria() {
    paginasLibres = tamBytes / tamPaginas;
    memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
}
