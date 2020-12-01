/*
    P R O G R A M A   P R I N C I P A L

*/

#include "SistemaMemoria.h"
#include <iostream>

using namespace std;

int main() {
    char opcion = ' ';
    int tamProceso, idProceso, dirVirtual, bitModificacion;
    SistemaMemoria memoriaAdminFIFO;
    SistemaMemoria memoriaAdminLRU(false);

    while(opcion != 'E'){
        cin >> opcion;
        opcion = toupper(opcion);

        switch (opcion) {
            // load proceso a memoria
            case 'P':
                cin >> tamProceso >> idProceso;
                cout << "Cargar PROCESO con ID " << idProceso << " de tamano " << tamProceso << endl;

                cout << "----------- POLITICA FIFO -------------------------" << endl;
                if (tamProceso <= 0) {
                    cout << "Tamano del proceso invalido, intente con otro" << endl;
                }
                else if (memoriaAdminFIFO.existeProceso(idProceso)) {
                    cout << "Ya existe un proceso con este nombre, intente con otro" << endl;
                }
                else if (!memoriaAdminFIFO.cargarProceso(idProceso, tamProceso)) {
                    cout << "No hay suficiente espacio para este proceso" << endl;
                }

                cout << "----------- POLITICA LRU -------------------------" << endl;
                if (tamProceso <= 0) {
                    cout << "Tamano del proceso invalido, intente con otro" << endl;
                }
                else if (memoriaAdminLRU.existeProceso(idProceso)) {
                    cout << "Ya existe un proceso con este nombre, intente con otro" << endl;
                }
                else if (!memoriaAdminLRU.cargarProceso(idProceso, tamProceso)) {
                    cout << "No hay suficiente espacio para este proceso" << endl;
                }
                break;
            //acceso a la dir virtual de proceso
            case 'A': 
                cin >> dirVirtual >> idProceso >> bitModificacion;
                if (bitModificacion == 1) cout << "Modificar ";
                else cout << "Acceder a ";
                cout << "direccion virtual " << dirVirtual << " del PROCESO con ID " << idProceso << endl;

                cout << "----------- POLITICA FIFO -------------------------" << endl;
                if (!memoriaAdminFIFO.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                }
                else if (dirVirtual < 0 || dirVirtual >= memoriaAdminFIFO.getProceso(idProceso).getTamBytes()) {
                    cout << "Direccion virtual invalida, intente con otra" << endl;
                } else {
                    memoriaAdminFIFO.accederDirVirtualProceso(dirVirtual, idProceso, bitModificacion);
                }

                cout << "----------- POLITICA LRU -------------------------" << endl;
                if (!memoriaAdminLRU.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                }
                else if (dirVirtual < 0 || dirVirtual >= memoriaAdminLRU.getProceso(idProceso).getTamBytes()) {
                    cout << "Direccion virtual invalida, intente con otra" << endl;
                } else {
                    memoriaAdminLRU.accederDirVirtualProceso(dirVirtual, idProceso, bitModificacion);
                }

                break;
            //liberar memoria real
            case 'L':
                cin >> idProceso;
                cout << "Liberar PROCESO con ID " << idProceso << endl;

                cout << "----------- POLITICA FIFO -------------------------" << endl;
                if (!memoriaAdminFIFO.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                } else {
                    memoriaAdminFIFO.liberarProceso(idProceso);
                }

                cout << "----------- POLITICA LRU -------------------------" << endl;
                if (!memoriaAdminLRU.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                } else {
                    memoriaAdminLRU.liberarProceso(idProceso);
                }

                break;
            //comentarios
            case 'C':  
                break;
            //limpiar y dar estadisticas
            case 'F': 
                cout << "Reiniciando memorias... mostrar estadisticas" << endl;
                memoriaAdminFIFO.limpiarMemorias();
                memoriaAdminLRU.limpiarMemorias();
                break;
            default:
                break;
        }//switch
    }//while

    return 0;
}
