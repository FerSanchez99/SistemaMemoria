#include "SistemaMemoria.h"
#include <iostream>
#include <math.h>
#include <utility>

/*
 * SistemaMemoria
 * Constructor default  
 */
SistemaMemoria::SistemaMemoria() { }

/*
 * SistemaMemoria
 * Constructor que asigna si la politica es FIFO
 * Params: 
 * - esFIFO bool true si es FIFO, false si es LRU
 */
SistemaMemoria::SistemaMemoria(bool esFIFO) {
    real.setPoliticaFIFO(esFIFO);
}

/*
 * getProceso
 * Recibe un id de un proceso y regresa el proceso con ese id de la lista de procesos en ejecucion
 * Params:
 * - idProceso int Un entero que indica el id de un proceso
 * Return:
 * - Proceso el proceso con id IdProceso en la lista de procesos  
 */
Proceso SistemaMemoria::getProceso(int idProceso) {
    return listaProcesos[idProceso];
}

/*
 * existeProceso
 * Checa si existe un proceso en ejecucion de acuerdo al id recibido
 * Params:
 * - idProceso int Un entero que indica el id de un proceso
 * Return:
 * - bool true si existe un proceso con ese id, false si no existe
 */
bool SistemaMemoria::existeProceso(int idProceso) {
    return listaProcesos.find(idProceso) != listaProcesos.end();
}

/*
 * Cargar proceso
 * Carga un proceso en memoria, regresa si hubo exito en la ejecucion
 * Params:
 * - idProceso int Un entero que indica el id de un proceso
 * - tamBytes int El tamano en bytes del proceso que se quiere asignar a memoria
 * Return:
 * - bool true si se asigno con exito, false si hubo algun fallo  
 */
bool SistemaMemoria::cargarProceso(int idProceso, int tamBytes) {
    // Crea un proceso
    Proceso proceso(idProceso, tamBytes);
    proceso.setTimeStart(stats.getTime());

    // Verifica que exista suficiente espacio para el proceso que se quiere asignar
    if (tamBytes > real.getTamBytes() ||
        (real.getPaginasLibres() + swap.getPaginasLibres()) < tamBytes / proceso.getTamPaginas() + (tamBytes % proceso.getTamPaginas() != 0)) {
        return false;
    }

    // Se agrega proceso a la lista de procesos en el programa
    listaProcesos[idProceso] = proceso;
    listaProcesos[idProceso].setTimeStart(stats.getTime()) ;

    //double para guardar el tiempo despu�s de cargar las p�ginas del nuevo proceso
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

/*
 * accederDirVirtualProceso
 * Busca en donde se encuentra la direccion virtual de un proceso en la direccio real, realiza swapIn si es necesario
 * Params:
 * - dirVirtual int La direccion virtual de un proceso
 * - idProceso int El id de un proceso
 * - bitModificacion int El bit que indica si se esta leyendo o modificando la direccion virtual
 */
void SistemaMemoria::accederDirVirtualProceso(int dirVirtual, int idProceso, int bitModificacion ) {
    Proceso proceso = listaProcesos[idProceso];

    int paginaVirtual = dirVirtual / proceso.getTamPaginas();
    auto pagProceso = proceso.getPagTablaDeMapeo(paginaVirtual);
    //si dicha pagina si esta en la memoria
    if (pagProceso.second) {
        if (!real.esPoliticaFIFO()) {
            real.aplicarLRU(pagProceso.first, idProceso);
        }
        std::cout << "Direccion virtual " << dirVirtual << " de PROCESO " << idProceso
                << " esta en la direccion real " << pagProceso.first * proceso.getTamPaginas() + (dirVirtual % proceso.getTamPaginas())
                << " de la Memoria REAL" << std::endl;

        //Acceder una pagina dentro de la memoria real se tarda 0.1s
        stats.setTime(stats.getTime()+0.1);
        return;
    }




    //Pair para llevar conteo de los swaps al acceder a una direcci�n first swap in, second swap out
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
        //si no encontro el proceso en vPF significa que es el primer page fault atribuido a ese proceso, crea un par correspondiente
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

/*
 * liberarProceso
 * Libera las paginas ocupadas por un proceso
 * Params:
 * - idProceso int El ID del proceso que se quiere liberar de memoria  
 */
void SistemaMemoria::liberarProceso(int idProceso) {
    //antes de liberar el proceso se calcula su Turnaround y se guarda junto con su id.
    stats.calcTA(listaProcesos[idProceso].getTimeStart(),idProceso);
    real.liberarProceso(listaProcesos[idProceso]);
    swap.liberarProceso(listaProcesos[idProceso]);
    listaProcesos.erase(idProceso);
}

/*
 * limpiarMemorias
 * Resetea todas las memorias y estadisticas  
 */
void SistemaMemoria::limpiarMemorias() {
    //se vuelven a restablecer las estadisticas junto con el tiempo
    stats.wipe();
    real.limpiarMemoria();
    swap.limpiarMemoria();
    listaProcesos.clear();
}



///////////////
//// Nombre: getStat()
//// Descripcion: Regresa las estadistica del sistema de memoria
//// Parametros: NO
//// Return: Stats stats
//////////////
Stats SistemaMemoria::getStat(){
    return stats;
}

