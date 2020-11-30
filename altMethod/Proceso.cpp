#include "Proceso.h"

Proceso::Proceso() { }

Proceso::Proceso(int id, int tamBytes) {
    this->id = id;
    this->tamBytes = tamBytes;

    tablaDeMapeo = std::vector<std::pair<int, bool>>(tamBytes / tamPaginas + (tamBytes % tamPaginas != 0), std::pair<int, bool>(0, false));
}

void Proceso::operator=(const Proceso& proceso) {
    id = proceso.id;
    tamBytes = proceso.tamBytes;
    tablaDeMapeo = proceso.tablaDeMapeo;
}

int Proceso::getId() {
    return id;
}

int Proceso::getTamPaginas() {
    return tamPaginas;
}

int Proceso::getTamBytes() {
    return tamBytes;
}

std::vector<std::pair<int, bool>> Proceso::getTablaDeMapeo() {
    return tablaDeMapeo;
}

std::pair<int, bool> Proceso::getPagTablaDeMapeo(int pagina) {
    return tablaDeMapeo[pagina];
}

void Proceso::setIndexTablaDeMapeo(int idx, std::pair<int, bool> par) {
    tablaDeMapeo[idx] = par;
}



///////////////
//// Nombre: getTimeStart()
//// Descripción: regresa el tiempo inicial del proceso
//// Parametros: NO
//// Return: double dTimeStart
//////////////
double Proceso::getTimeStart(){
    return dTimeStart;
}

///////////////
//// Nombre: setTimeStart()
//// Descripción: Guada el tiempo inicial del proceso
//// Parametros: time
//// Return: NO
//////////////
void Proceso::setTimeStart(double time){
    dTimeStart = time;
}
