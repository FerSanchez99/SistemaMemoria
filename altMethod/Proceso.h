#ifndef PROCESO_H
#define PROCESO_H

#include <vector>
#include <utility>

class Proceso {
    private:
        const int tamPaginas = 16; // tamano de una pagina 
        int id; // id del proceso
        int tamBytes; // tamano en bytes del proceso a cargar
        std::vector<std::pair<int, bool>> tablaDeMapeo;
        double dTimeStart; // tiempo en que se cargo el proceso a la memoria real


    public:
        Proceso(); // Constructor default
        Proceso(int, int); // Constructor con id y tamano en bytes
        void operator=(const Proceso&); // Overload operador de asignacion
        int getId(); // Regresa el id
        int getTamPaginas(); // Regresa el tamano de pagina que se esta utilizando
        int getTamBytes(); // Regresa el tamano total del proceso
        double getTimeStart(); // regresa el tiempo en que se cargo el proceso
        void setTimeStart(double); // establece el tiempo incial del proceso

        // Tabla de mapeo, indice indica no. de pagina virtual, 
        // el pair.first indica direccion en memoria real o swap y pair.second indica si esta en memoria real o no 
        std::vector<std::pair<int, bool>> getTablaDeMapeo();
        std::pair<int, bool> getPagTablaDeMapeo(int); // Regresa el par de ese indice
        void setIndexTablaDeMapeo(int, std::pair<int, bool>); // Set un par en el indice de acuerdo al indice que se recibe

};

#endif
