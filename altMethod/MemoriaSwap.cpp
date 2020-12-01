#include "MemoriaSwap.h"
#include <utility>
#include <iostream>
using namespace std;

/*
 * MemoriaSwap
 * Constructor Default
 */
MemoriaSwap::MemoriaSwap() {}

/*
 * getTamBytes
 * Regresa el tamano total de la memoria swap en bytes
 * Return:
 * - tamBytes int El tamano total de la memoria en bytes
 */
int MemoriaSwap::getTamBytes() {
    return tamBytes;
}

/*
 * getPaginasLibres
 * Regresa numero de paginas libres en memoria
 * Return:
 * - int El numero de paginas libres, disponibles
 */
int MemoriaSwap::getPaginasLibres() {
    return paginasLibres;
}

/*
 * setPaginasLibres
 * Set el numero de paginas libres
 * Params:
 * - paginasLibres int El numero de paginas libres, disponibles
 */
void MemoriaSwap::setPaginasLibres(int paginasLibres) {
    this->paginasLibres = paginasLibres;
}

/*
 * asignarPagina
 * Asigna un pagina virtual de un proceso en una pagina libre en memoria, y regresa el indice en la que se asigno   
 * Params:
 * - paginaVirtual int El numero de pagina virtual de un proceso
 * - idProceso int El ID de un proceso
 * Return:
 * - int Regresa el indice que se asigno en memoria, -1 si no se encontro pagina libre
 */
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

/*
 * swapIn
 * Realiza un swapIn hacia memoria real   
 * Params:
 * - proceso Proceso Un proceso
 * - real MemoriaReal La memoria real
 * - paginaVirtual int La pagina virtual que se desea swapear
 * - listaProcesos unordered_map<int,int> La lista de procesos
 * - dtime double El tiempo actual de ejecucion
 * - swaps pair<int, int> El no de swaps in y swaps outs totales
 */
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
        //Se actualiza el tiempo, cada swap in toma 1s
        dTime+=1.0;
        //Se actiliza swaps.first, el # de swaps in
        swaps.first++;
        return;
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
            //Se actualiza el tiempo 1 seg por el swap out y un 1 seg por el swap in
            dTime+=2;
            //Se actuliza swaps.first, el # de swaps in y swaps.second, el # de swaps out
            swaps.first++;
            swaps.second++;

}

/*
 * liberarProceso
 * Liberar un proceso de la memoria real   
 * Params:
 * - proceso Proceso El proceso que se quiere liberar
 */
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

/*
 * limpiarMemoria
 * Limpiar toda la memoria, es decir, es un reseteo total
 */
void MemoriaSwap::limpiarMemoria() {
    paginasLibres = tamBytes / tamPaginas;
    memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
}
