/*
    P R O G R A M A   P R I N C I P A L

*/

#include "SistemaMemoria.h"
#include <iostream>

using namespace std;

int main() {
    char opcion = ' ';
    int tamProceso, idProceso, dirVirtual, bitModificacion;
    SistemaMemoria memoriaAdmin;

    while(opcion != 'E'){
        cin >> opcion;
        opcion = toupper(opcion);

        switch (opcion) {
            // load proceso a memoria
            case 'P':
                cin >> tamProceso >> idProceso;
                
                if (tamProceso <= 0) {
                    cout << "Tamano del proceso invalido, intente con otro" << endl;
                    break;
                }
                else if (memoriaAdmin.existeProceso(idProceso)) {
                    cout << "Ya existe un proceso con este nombre, intente con otro" << endl;
                    break;
                }
                else if (!memoriaAdmin.cargarProceso(idProceso, tamProceso)) {
                    cout << "No hay suficiente espacio para este proceso" << endl;
                }
                break;
            //acceso a la dir virtual de proceso
            case 'A': 
                cin >> dirVirtual >> idProceso >> bitModificacion;

                if (!memoriaAdmin.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                    break;
                }
                else if (dirVirtual < 0 || dirVirtual >= memoriaAdmin.getProceso(idProceso).getTamBytes()) {
                    cout << "Direccion virtual invalida, intente con otra" << endl;
                    break;
                }

                memoriaAdmin.accederDirVirtualProceso(dirVirtual, idProceso, bitModificacion);
                break;
            //liberar memoria real
            case 'L':
                cin >> idProceso;
                if (!memoriaAdmin.existeProceso(idProceso)) {
                    cout << "No existe proceso con este id, intente con otro" << endl;
                    break;
                }

                memoriaAdmin.liberarProceso(idProceso);
                break;
            //comentarios
            case 'C':  
                break;
            //limpiar y dar estadisticas
            case 'F':  
                memoriaAdmin.limpiarMemorias();
                break;
            default:
                break;
        }//switch
    }//while

    return 0;
}
