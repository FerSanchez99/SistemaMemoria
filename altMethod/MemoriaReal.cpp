#include "MemoriaReal.h"
#include <iostream>

MemoriaReal::MemoriaReal() {}

int MemoriaReal::getPaginasLibres() {
    return paginasLibres;
}

void MemoriaReal::setPaginasLibres(int paginasLibres) {
    this->paginasLibres = paginasLibres;
}

int MemoriaReal::asignarPagina(int paginaVirtual, int idProceso) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].second == -1) {
            FIFO.push_back(std::make_pair(i, idProceso));
            memoria[i].first = paginaVirtual;
            memoria[i].second = idProceso;
            paginasLibres--;

            return i;
        }
    }

    return -1;
}

std::pair<int, std::pair<int, int>> MemoriaReal::intercambiaRealSwap(int paginaVirtual, int idProceso) {
    auto front = FIFO.front();
    FIFO.pop_front();
    auto intercambioReal = std::make_pair(front.first, memoria[front.first]);

    FIFO.push_back(std::make_pair(front.first, idProceso));
    memoria[front.first].first = paginaVirtual;
    memoria[front.first].second = idProceso;

    return intercambioReal;
}

void MemoriaReal::asignarProceso(Proceso& proceso, MemoriaSwap& swap, std::unordered_map<int, Proceso>& listaProcesos) {
    int paginasProceso = proceso.getTablaDeMapeo().size();
    int idProceso = proceso.getId();
    
    if (paginasProceso <= paginasLibres) {
        for (int i = 0, pags = 0; i < memoria.size() && pags < paginasProceso; i++) {
            if (memoria[i].second == -1) {
                FIFO.push_back(std::make_pair(i, idProceso));
                memoria[i].first = pags;
                memoria[i].second = idProceso;
                proceso.setIndexTablaDeMapeo(pags, std::make_pair(i, true));
                pags++;
                paginasLibres--;

                std::cout << "Se asigno proceso " << idProceso << "en memoria real en pagina " << i << std::endl;
            }
        }

        return;
    }

    int pags = 0;
    for (int i = 0; i < memoria.size() && paginasLibres > 0; i++) {
        if (memoria[i].second == -1) {
            FIFO.push_back(std::make_pair(i, idProceso));
            memoria[i].first = pags;
            memoria[i].second = idProceso;
            proceso.setIndexTablaDeMapeo(pags, std::make_pair(i, true));
            pags++;
            paginasLibres--;

            std::cout << "Se asigno proceso " << idProceso << "en memoria real en pagina " << i << std::endl;
        }
    }

    while (pags < paginasProceso) {
        auto front = FIFO.front();
        FIFO.pop_front();
        
        swapOut(swap, listaProcesos[front.second], front);

        memoria[front.first].first = pags;
        memoria[front.first].second = idProceso;
        proceso.setIndexTablaDeMapeo(pags, std::make_pair(front.first, true));

        std::cout << "Se asigno proceso " << idProceso << "en memoria real en pagina " << front.first << std::endl;

        FIFO.push_back(std::make_pair(front.first, idProceso));
        pags++;
    }
}

void MemoriaReal::swapOut(MemoriaSwap& swap, Proceso& proceso, std::pair<int, int> paginaReal) {
    int paginaSwap = swap.asignarPagina(memoria[paginaReal.first].first, paginaReal.second);

    proceso.setIndexTablaDeMapeo(memoria[paginaReal.first].first, std::make_pair(paginaSwap, false));

    std::cout << "SwapOut - proceso " << paginaReal.second << "de memoria real en pagina " << paginaReal.first 
            << " se movio a memoria swap en pagina " << paginaSwap << std::endl;
}

void MemoriaReal::liberarProceso(Proceso proceso) {
    auto tablaProceso = proceso.getTablaDeMapeo();
    int idProceso = proceso.getId();
    
    for (auto dir_enMemoria : tablaProceso) {
        if (dir_enMemoria.second) {
            paginasLibres++;
            memoria[dir_enMemoria.first] = std::make_pair(-1, -1);

            std::cout << "Se libero proceso " << idProceso << "en memoria real en pagina " << dir_enMemoria.first << std::endl;
        }
    }

    FIFO.remove_if([idProceso](std::pair<int, int> par){
        return par.second == idProceso;
    });
}