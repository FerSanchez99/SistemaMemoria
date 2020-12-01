#include "MemoriaReal.h"
#include <iostream>

/*
 * MemoriaReal
 * Constructor default, esFIFO es true  
 */
MemoriaReal::MemoriaReal() {
    esFIFO = true;
}

/*
 * SistemaMemoria
 * Constructor que asigna si es FIFO o no
 * Params:
 * - esFIFO bool true si es la politica es FIFO, false si es LRU  
 */
MemoriaReal::MemoriaReal(bool esFIFO) {
    this->esFIFO = esFIFO;
}

/*
 * esPoliticaFIFO
 * Regresa si la politica de reemplazo es FIFO o no
 * Return:
 * - bool true si es FIFO, false si es LRU  
 */
bool MemoriaReal::esPoliticaFIFO() {
    return esFIFO;
}

/*
 * setPoliticaFIFO
 * Set si es FIFO o no
 * Params:
 * - esFIFO bool true si es FIFO, false si es LRU  
 */
void MemoriaReal::setPoliticaFIFO(bool esFIFO) {
    this->esFIFO = esFIFO;
}

/*
 * getTamBytes
 * Regresa tamano total de la memoria en bytes
 * Return:
 * - int Tamano total de la memoria en bytes  
 */
int MemoriaReal::getTamBytes() {
    return tamBytes;
}

/*
 * getPaginasLibres
 * Regresa el numero de paginas libres
 * Return:
 * - int Numero de paginas libres  
 */
int MemoriaReal::getPaginasLibres() {
    return paginasLibres;
}

/*
 * setPaginasLibres
 * Asigna el numero de paginas libres
 * Params: 
 * - paginasLibres int Numero de paginas libres  
 */
void MemoriaReal::setPaginasLibres(int paginasLibres) {
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

/*
 * IntercambiaRealSwap
 * Regresa una pagina de memoria real, (indice de pagina real, (indice de pagina virtual, idProceso))
 * Params:
 * - paginaVirtual int Es el indice de pagina virtual de un proceso
 * - idProceso int Es el ID de un proceso
 * Return:
 * - pair<int, pair<int, int>> Una pagina de memoria real, (indice de pagina real, (indice de pagina virtual, idProceso))
 */
std::pair<int, std::pair<int, int>> MemoriaReal::intercambiaRealSwap(int paginaVirtual, int idProceso) {
    auto front = politicaRemplazoLista.front();
    politicaRemplazoLista.pop_front();
    auto intercambioReal = std::make_pair(front.first, memoria[front.first]);

    politicaRemplazoLista.push_back(std::make_pair(front.first, idProceso));
    memoria[front.first].first = paginaVirtual;
    memoria[front.first].second = idProceso;

    return intercambioReal;
}

/*
 * asignarProceso
 * Asigna un proceso en memoria real   
 * Params:
 * - proceso Proceso El proceso que se quiere asignar
 * - swap MemoriaSwap La memoria swap en caso de ser necesario realizar swapOuts
 * - listaProcesos unordered_map<int, Proceso> Un mapa que mapea idProceso con su respectivo Proceso
 * - dTime double El tiempo actual del programa
 * - swaps int El numero de swaps que se han realizado
 */
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

        //se suma el tiempo que se tardo en cargar a memoria real 1s por cada p�gina
        dTime = dTime + (paginasProceso*1.0);
        return;
    }

    //meter las paginas del proceso que si alcanzan lugar en la memoria real antes de un swap out
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
            //se actualiza el tiempo sumando 1s por las p�ginas que si alcanzaron cupo
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
        //Se actualiza el tiempo 1s por es swap out y 1s por el load a memoria real
        dTime = dTime + 2.0;
        //Se actualiza la cantidad de swaps que hubo el swap in y swap out ocurren en pares
        swaps++;
        politicaRemplazoLista.push_back(std::make_pair(front.first, idProceso));
        pags++;
    }
}

/*
 * swapOut
 * Realiza un swapOut hacia la memoria swap   
 * Params:
 * - swap MemoriaSwap Es la memoria swap hacia la que se hace el swapOut
 * - proceso Proceso Es el proceso al que se le esta haciendo swapOut a sus paginas
 * - paginaReal<int, int> es una paginaReal de memoria real, .first tiene indice de pagina real, .second tiene id del proceso que estaba en esa pagina
 */
void MemoriaReal::swapOut(MemoriaSwap& swap, Proceso& proceso, std::pair<int, int> paginaReal) {
    int paginaSwap = swap.asignarPagina(memoria[paginaReal.first].first, paginaReal.second);

    proceso.setIndexTablaDeMapeo(memoria[paginaReal.first].first, std::make_pair(paginaSwap, false));

    std::cout << "SwapOut - PROCESO " << paginaReal.second << " de Memoria REAL en pagina " << paginaReal.first
            << " se movio a Memoria SWAP en pagina " << paginaSwap << std::endl;
}

/*
 * liberarProceso
 * Libera un proceso de memoria
 * Params:
 * - proceso Proceso El proceso que se quiere liberar
 */
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

    // Quita de la politica de reemplazo los valores que estaba en uso por el proceso
    politicaRemplazoLista.remove_if([idProceso](std::pair<int, int> par){
        return par.second == idProceso;
    });
}

/*
 * limpiarMemoria
 * Limpia la memoria, es un reset de todos los atributos de esta
 */
void MemoriaReal::limpiarMemoria() {
    politicaRemplazoLista.clear();
    paginasLibres = tamBytes / tamPaginas;
    memoria = std::vector<std::pair<int, int>>(paginasLibres, std::pair<int, int>(-1, -1));
}

/*
 * aplicarLRU
 * Aplica politica de reemplazo LRU, mueve pagina mas recientemente usada al final de la lista   
 * Params:
 * - pagReal int El indice que indica la pagina real en memoria
 * - idProceso int El id del proceso al que pertenece la pagina
 */
void MemoriaReal::aplicarLRU(int pagReal, int idProceso) {
    politicaRemplazoLista.remove(std::make_pair(pagReal, idProceso));
    politicaRemplazoLista.push_back(std::make_pair(pagReal, idProceso));
}
