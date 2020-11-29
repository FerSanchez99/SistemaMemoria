#include "SistemaMemoria.h"
#include <iostream>
#include <math.h>
#include <utility>

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
    proceso.setTimeStart(stats.getTime());


    if (tamBytes > real.getTamBytes() ||
        (real.getPaginasLibres() + swap.getPaginasLibres()) < tamBytes / proceso.getTamPaginas() + (tamBytes % proceso.getTamPaginas() != 0)) {
        return false;
    }

    listaProcesos[idProceso] = proceso;
    listaProcesos[idProceso].setTimeStart(stats.getTime()) ;

    double temp = stats.getTime();
    int swaps=0;
    real.asignarProceso(listaProcesos[idProceso], swap, listaProcesos,temp,swaps);
    stats.setTime(temp);
    stats.setCantSwaps(stats.getCantSwaps().first,stats.getCantSwaps().second+swaps);


    return true;

}

void SistemaMemoria::accederDirVirtualProceso(int dirVirtual, int idProceso, int bitModificacion) {
    Proceso proceso = listaProcesos[idProceso];

    int paginaVirtual = dirVirtual / proceso.getTamPaginas();
    auto pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    //si dicha página sí está en la memoria
    if (pagProceso.second) {
        if (!real.esPoliticaFIFO()) {
            real.aplicarLRU(pagProceso.first, idProceso);
        }
        std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
                << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
                << " de la Memoria REAL" << std::endl;
        stats.setTime(stats.getTime()+0.1);
        return;
    }




    //Pair para llevar conteo de los swaps al acceder a una dirección first swap in, second swap out
    pair<int,int> swaps(0,0);
    double temp = stats.getTime();

    swap.swapIn(listaProcesos[idProceso], real, paginaVirtual, listaProcesos, temp,swaps);
    stats.setTime(temp);
    stats.setCantSwaps(stats.getCantSwaps().first+swaps.first,stats.getCantSwaps().second+swaps.second);
     //si no está en la memoria ocurre un page fault
    bool b=true;

    for(int i=0;i<stats.getPF().size();i++){

        if(idProceso==stats.getPF()[i].first) {
            stats.addPF(i);
            b=false;
            break;
        }
    }
    if(b == true)
        stats.pushb(make_pair(idProceso,1));




    proceso = listaProcesos[idProceso];
    paginaVirtual = dirVirtual / proceso.getTamPaginas();
    pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
            << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
            << " de la Memoria REAL" << std::endl;
}

void SistemaMemoria::liberarProceso(int idProceso) {

    stats.calcTA(listaProcesos[idProceso].getTimeStart(),idProceso);
    real.liberarProceso(listaProcesos[idProceso]);
    swap.liberarProceso(listaProcesos[idProceso]);
    listaProcesos.erase(idProceso);
}

void SistemaMemoria::limpiarMemorias() {

    real.limpiarMemoria();
    swap.limpiarMemoria();
    listaProcesos.clear();
}

Stats SistemaMemoria::getStat(){
    return stats;
}

