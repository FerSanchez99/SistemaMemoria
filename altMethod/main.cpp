/*
    P R O G R A M A   P R I N C I P A L
    Simular un administrador de memoria que recibe procesos, los asigna en memoria real
    y realiza los swapIns y swapOuts que sean necesarios. Se incluye un manejador con
    politica FIFO y uno de politica LRU.

    Equipo de desarrollo:
    Angel Guevara - A01570288
    Ma. Fernanda Sanchez - A01570306
    Mateo Espinosa - A00823972
*/

#include "SistemaMemoria.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Funcion para validar el input que se recibe
void validInput(char &opcion, int &tamProceso, int &idProceso, int &dirVirtual, int &bitModificacion, string &comment) {
    string line;
    getline(cin, line);

    stringstream ss(line);
    string s;

    // Leer primer caracter
    // Verificar que sea solamente un char
    if (!(ss >> s)) {
        cout << "Mal input, intente de nuevo" << endl;
        opcion = '-';
        return;
    }

    if (s.size() != 1) {
        cout << "Mal input, intente de nuevo" << endl;
        opcion = '-';
        return;
    }

    opcion = s[0];
    opcion = toupper(opcion);
    switch (opcion) {
        case 'P':
            // Verificar que los siguientes dos elementos sean enteros
            if (!(ss >> tamProceso)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            if (!(ss >> idProceso)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            return;
        case 'A':
            // Verificar que los siguientes elementos sean enteros
            if (!(ss >> dirVirtual)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            if (!(ss >> idProceso)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            if (!(ss >> bitModificacion)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            return;
        case 'L':
            // Verificar que sea entero
            if (!(ss >> idProceso)) {
                cout << "Mal input, intente de nuevo" << endl;
                opcion = '-';
                return;
            }
            return;
        case 'C':
            comment = ss.str();
            if (comment.size() < 3) {
                comment = "";
                return;
            }
            comment = comment.substr(2);
            return;
        case 'F':
            return;
        case 'E':
            return;
        default:
            cout << "Mal input, intente de nuevo" << endl;
            opcion = '-';
            return;
    }
}

int main() {
    char opcion = ' ';
    int tamProceso, idProceso, dirVirtual, bitModificacion;
    SistemaMemoria memoriaAdminFIFO;
    SistemaMemoria memoriaAdminLRU(false);
    string sComment;

    while(opcion != 'E'){
        validInput(opcion, tamProceso, idProceso, dirVirtual, bitModificacion, sComment);

        switch (opcion) {

            // load proceso a memoria
            case 'P':
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
                cout<<sComment<<endl;

                break;
            //limpiar y dar estadisticas
            case 'F':
                cout << "Reiniciando memorias... mostrar estadisticas" << endl;

                cout<<"///////////////// FIFO \\\\\\\\\\\\\\\\\\"<<endl;
                memoriaAdminFIFO.getStat().printStats();

                cout<<"///////////////// LRU  \\\\\\\\\\\\\\\\\\"<<endl;
                memoriaAdminLRU.getStat().printStats();




                memoriaAdminFIFO.limpiarMemorias();
                memoriaAdminLRU.limpiarMemorias();


                break;
            default:
                break;
        }//switch
    }//while

    cout<<" F I N   D E L   P R O G R A M A"<<endl;


    return 0;
}
