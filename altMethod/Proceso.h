#ifndef PROCESO_H
#define PROCESO_H

#include <vector>
#include <utility>

class Proceso {
    private:
        const int tamPaginas = 16;
        int id; //id del proceso
        int tamBytes; //tamaño en bytes del proceso a cargar
        std::vector<std::pair<int, bool>> tablaDeMapeo;
        double dTimeStart; //tiempo en que se cargó el proceso a la memoria real


    public:
        Proceso();
        Proceso(int, int);
        void operator=(const Proceso&);
        int getId();
        int getTamPaginas();
        int getTamBytes();
        double getTimeStart(); // regresa el tiempo en que se cargó el proceso
        void setTimeStart(double); // establece el tiempo incial del proceso


        std::vector<std::pair<int, bool>> getTablaDeMapeo();
        std::pair<int, bool> getPagTablaDeMapeo(int);
        void setIndexTablaDeMapeo(int, std::pair<int, bool>);

};

#endif
