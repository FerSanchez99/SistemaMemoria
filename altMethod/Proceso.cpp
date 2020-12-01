#include "Proceso.h"

/*
 * Proceso
 * Constructor default vacio
 */
Proceso::Proceso() { }

/*
 * Proceso
 * Proceso con id y tamano deseado
 * Params:
 * - id int El id del proceso
 * - tamBytes int El tamano en bytes del proceso
 */
Proceso::Proceso(int id, int tamBytes) {
    this->id = id;
    this->tamBytes = tamBytes;

    tablaDeMapeo = std::vector<std::pair<int, bool>>(tamBytes / tamPaginas + (tamBytes % tamPaginas != 0), std::pair<int, bool>(0, false));
}

/*
 * operator=
 * Sobrecarga el operador= para asignar un proceso   
 * Params:
 * - proceso Proceso Un proceso
 */
void Proceso::operator=(const Proceso& proceso) {
    id = proceso.id;
    tamBytes = proceso.tamBytes;
    tablaDeMapeo = proceso.tablaDeMapeo;
}

/*
 * getId
 * Regresa el id del proceso
 * Return:
 * - int El id del proceso
 */
int Proceso::getId() {
    return id;
}

/*
 * getTamPaginas
 * Regresa que tiene una pagina del proceso
 * Return:
 * - int El tamano de una pagina
 */
int Proceso::getTamPaginas() {
    return tamPaginas;
}

/*
 * getTamBytes
 * Regresa el tamano total en bytes del proceso
 * Return:
 * - int Tamano total en bytes del proceso
 */
int Proceso::getTamBytes() {
    return tamBytes;
}

/*
 * getTablaDeMapeo
 * Regresa tabla de mapeo: indice indica no. de pagina virtual,
 * el pair.first indica direccion en memoria real o swap y pair.second indica si esta en memoria real o no 
 * Return:
 * - vector<std::pair<int, bool>> Tabla de Mapeo
 */
std::vector<std::pair<int, bool>> Proceso::getTablaDeMapeo() {
    return tablaDeMapeo;
}

/*
 * getPagTablaDeMapeo
 * Regresa el par de ese indice de la tabla de mapeo
 * Params:
 * - pagina int Indice de la tabla de mapeo
 * Return:
 * - pair<int, bool> Regresa el indice que se asigno en memoria, -1 si no se encontro pagina libre
 */
std::pair<int, bool> Proceso::getPagTablaDeMapeo(int pagina) {
    return tablaDeMapeo[pagina];
}

/*
 * setIndexTablaDeMapeo
 * Set un par en el indice de acuerdo al indice que se recibe   
 * Params:
 * - idx int Indice de la tabla de mapeo, pagina virtual del proceso
 * - par pair<int, bool> pair.first indica direccion en memoria real o swap y pair.second indica si esta en memoria real o no 
 * Return:
 * - int Regresa el indice que se asigno en memoria, -1 si no se encontro pagina libre
 */
void Proceso::setIndexTablaDeMapeo(int idx, std::pair<int, bool> par) {
    tablaDeMapeo[idx] = par;
}



///////////////
//// Nombre: getTimeStart()
//// Descripcion: regresa el tiempo inicial del proceso
//// Parametros: NO
//// Return: double dTimeStart
//////////////
double Proceso::getTimeStart(){
    return dTimeStart;
}

///////////////
//// Nombre: setTimeStart()
//// Descripcion: Guada el tiempo inicial del proceso
//// Parametros: time
//// Return: NO
//////////////
void Proceso::setTimeStart(double time){
    dTimeStart = time;
}
