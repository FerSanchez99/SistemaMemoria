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

    //double para guardar el tiempo después de cargar las páginas del nuevo proceso
    double temp = stats.getTime();
    //int para guardar la cantidad swaps out que hubo, no hay swaps in cuando se carga un proceso nuevo a memoria
    int swaps=0;

    real.asignarProceso(listaProcesos[idProceso], swap, listaProcesos,temp,swaps);
    //se actualiza el valor del tiempo
    stats.setTime(temp);
    //se actualiza el valor de swaps out
    stats.setCantSwaps(stats.getCantSwaps().first,stats.getCantSwaps().second+swaps);


    return true;

}

void SistemaMemoria::accederDirVirtualProceso(int dirVirtual, int idProceso, int bitModificacion ) {
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

        //Acceder una página dentro de la memoria real se tarda 0.1s
        stats.setTime(stats.getTime()+0.1);
        return;
    }




    //Pair para llevar conteo de los swaps al acceder a una dirección first swap in, second swap out
    pair<int,int> swaps(0,0);
    //double para guarda el tiempo despues de hacer los swaps in y swaps out necesarios
    double temp = stats.getTime();

    swap.swapIn(listaProcesos[idProceso], real, paginaVirtual, listaProcesos, temp,swaps);

    //Se guarda el nuevo tiempo
    stats.setTime(temp);
    //Se guarda la cantidad de swaps que hubo
    stats.setCantSwaps(stats.getCantSwaps().first+swaps.first,stats.getCantSwaps().second+swaps.second);


    //Si swaps.first tiene un valor no cero significa que hubo swaps in y por ende page faults
    if(swaps.first != 0){
        bool b=true;

        //Siguiente bucle busca en vPF si existe ya un par para el proceso con id = idProceso y es asi b = false y guarda el nuevo valor del par.second
        for(int i=0;i<stats.getPF().size();i++){

            if(idProceso==stats.getPF()[i].first) {
                stats.addPF(i);
                b=false;
                break;
            }
        }
        //si no encontró el proceso en vPF significa que es el primer page fault atribuido a ese proceso, crea un par correspondiente
        if(b)
            stats.pushb(make_pair(idProceso,1));
    }




    proceso = listaProcesos[idProceso];
    paginaVirtual = dirVirtual / proceso.getTamPaginas();
    pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
            << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
            << " de la Memoria REAL" << std::endl;
}

void SistemaMemoria::liberarProceso(int idProceso) {
    //antes de liberar el proceso se calcula su Turnaround y se guarda junto con su id.
    stats.calcTA(listaProcesos[idProceso].getTimeStart(),idProceso);
    real.liberarProceso(listaProcesos[idProceso]);
    swap.liberarProceso(listaProcesos[idProceso]);
    listaProcesos.erase(idProceso);
}

void SistemaMemoria::limpiarMemorias() {
    //se vuelven a restablecer las estadísticas junto con el tiempo
    stats.wipe();
    real.limpiarMemoria();
    swap.limpiarMemoria();
    listaProcesos.clear();
}



///////////////
//// Nombre: getStat()
//// Descripción: Regresa las estadística del sistema de memoria
//// Parametros: NO
//// Return: Stats stats
//////////////
Stats SistemaMemoria::getStat(){
    return stats;
}

