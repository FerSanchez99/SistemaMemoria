#ifndef PROCESO_H
#define PROCESO_H

#include <vector>
#include <utility>

class Proceso {
    private:
        const int tamPaginas = 16;
        int id;
        int tamBytes;
        std::vector<std::pair<int, bool>> tablaDeMapeo;
    public:
        Proceso();
        Proceso(int, int);
        void operator=(const Proceso&);
        int getId();
        int getTamPaginas();
        int getTamBytes();
        std::vector<std::pair<int, bool>> getTablaDeMapeo();
        std::pair<int, bool> getPagTablaDeMapeo(int);
        void setIndexTablaDeMapeo(int, std::pair<int, bool>);
};

#endif