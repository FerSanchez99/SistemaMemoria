#include "Proceso.h"

Proceso::Proceso() { }

Proceso::Proceso(int id, int tamBytes) {
    this->id = id;
    this->tamBytes = tamBytes;
    tablaDeMapeo = std::vector<std::pair<int, bool>>(tamBytes / tamPaginas + (tamBytes % tamPaginas != 0), std::pair<int, bool>(0, false));
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

void Proceso::setIndexTablaDeMapeo(int idx, std::pair<int, bool> par) {
    tablaDeMapeo[idx] = par;
}