#include "MemoriaReal.h"
#include <iostream>

MemoriaReal::MemoriaReal() {
    esFIFO = true;
}

MemoriaReal::MemoriaReal(bool esFIFO) {
    this->esFIFO = esFIFO;
}

bool MemoriaReal::esPoliticaFIFO() {
    return esFIFO;
}

void MemoriaReal::setPoliticaFIFO(bool esFIFO) {
    this->esFIFO = esFIFO;
}

int MemoriaReal::getTamBytes() {
    return tamBytes;
}

int MemoriaReal::getPaginasLibres() {
    return paginasLibres;
}

void MemoriaReal::setPaginasLibres(int paginasLibres) {
    this->paginasLibres = paginasLibres;
}

int MemoriaReal::asignarPagina(int paginaVirtual, int idProceso) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].second == -1) {
            politicaRemplazoLista.push_back(std::make_pair(i, idProceso));
            memoria[i].first = paginaVirtual;
            memoria[i].second = idProceso;
            paginasLibres--;

            return i;
        }
    }

    return -1;
}

std::pair<int, std::pair<int, int>> MemoriaReal::intercambiaRealSwap(int paginaVirtual, int idProceso) {
    auto front = politicaRemplazoLista.front();
    politicaRemplazoLista.pop_front();
    auto intercambioReal = std::make_pair(front.first, memoria[front.first]);

    politicaRemplazoLista.push_back(std::make_pair(front.first, idProceso));
    memoria[front.first].first = paginaVirtual;
    memoria[front.first].second = idProceso;

    return intercambioReal;
}

void MemoriaReal::asignarProceso(Proceso& proceso, MemoriaSwap& swap, std::unordered_map<int, Proceso>& listaProcesos, double& dTime, int& swaps) {
    int paginasProceso = proceso.getTablaDeMapeo().size();
    int idProceso = proceso.getId();

    //no swap
    if (paginasProceso <= paginasLibres) {
        for (int i = 0, pags = 0; i < memoria.size() && pags < paginasProceso; i++) {
            if (memoria[i].second == -1) {
                politicaRemplazoLista.push_back(std::make_pair(i, idProceso));
                memoria[i].first = pags;
                memoria[i].second = idProceso;
                proceso.setIndexTablaDeMapeo(pags, std::make_pair(i, true));
                pags++;
                paginasLibres--;

                std::cout << "Se asigno PROCESO " << idProceso << " en Memoria REAL en pagina " << i << std::endl;
            }
        }

        //se suma el tiempo que se tardo en cargar a memoria real
        dTime = dTime + (paginasProceso*1.0);
        return;
    }

    //meter las paginas del proceso si alcanzan lugar en la memoria real antes de un swap out
    int pags = 0;
    for (int i = 0; i < memoria.size() && paginasLibres > 0; i++) {
        if (memoria[i].second == -1) {
            politicaRemplazoLista.push_back(std::make_pair(i, idProceso));
            memoria[i].first = pags;
            memoria[i].second = idProceso;
            proceso.setIndexTablaDeMapeo(pags, std::make_pair(i, true));
            pags++;
            paginasLibres--;

            std::cout << "Se asigno PROCESO " << idProceso << " en Memoria REAL en pagina " << i << std::endl;
            dTime = dTime + 1.0;
        }
    }

    //swap out de memoria
    while (pags < paginasProceso) {
        auto front = politicaRemplazoLista.front();
        politicaRemplazoLista.pop_front();

        swapOut(swap, listaProcesos[front.second], front);

        memoria[front.first].first = pags;
        memoria[front.first].second = idProceso;
        proceso.setIndexTablaDeMapeo(pags, std::make_pair(front.first, true));

        std::cout << "Se asigno PROCESO " << idProceso << " en Memoria REAL en pagina " << front.first << std::endl;
        dTime = dTime + 2.0; //1 segundo por swap out y 1 segundo por loadear pagina
        swaps++;
        politicaRemplazoLista.push_back(std::make_pair(front.first, idProceso));
        pags++;
    }
}

void MemoriaReal::swapOut(MemoriaSwap& swap, Proceso& proceso, std::pair<int, int> paginaReal) {
    int paginaSwap = swap.asignarPagina(memoria[paginaReal.first].first, paginaReal.second);

    proceso.setIndexTablaDeMapeo(memoria[paginaReal.first].first, std::make_pair(paginaSwap, false));

    std::cout << "SwapOut - PROCESO " << paginaReal.second << " de Memoria REAL en pagina " << paginaReal.first
            << " se movio a Memoria SWAP en pagina " << paginaSwap << std::endl;
}

void MemoriaReal::liberarProceso(Proceso proceso) {
    auto tablaProceso = proceso.getTablaDeMapeo();
    int idProceso = proceso.getId();

    for (auto dir_enMemoria : tablaProceso) {
        if (dir_enMemoria.second) {
            paginasLibres++;
            memoria[dir_enMemoria.first] = std::make_pair(-1, -1);

            std::cout << "Se libero PROCESO " << idProceso << " en Memoria REAL en pagina " << dir_enMemoria.first << std::endl;
        }
    }

    politicaRemplazoLista.remove_if([idProceso](std::pair<int, int> par){
        return par.second == idProceso;
    });
}

void MemoriaReal::limpiarMemoria() {
    politicaRemplazoLista.clear();
    paginasLibres = tamBytes / tamPaginas;
    memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
}

void MemoriaReal::aplicarLRU(int pagReal, int idProceso) {
    politicaRemplazoLista.remove(std::make_pair(pagReal, idProceso));
    politicaRemplazoLista.push_back(std::make_pair(pagReal, idProceso));
}
