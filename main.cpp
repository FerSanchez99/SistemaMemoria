/*
    P R O G R A M A   P R I N C I P A L

*/


#include "mem.h"
#include "Proceso.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

mem MemoriaReal[2048];
mem MemoriaRealLRU[2048];
vector<Proceso> ListaProcesos;


void LoadInMem(int iTam, int iP, int iNumMarco){


    vector<pair<int,bool>> vB;

    double iCantMarcos = iTam/16.0; //calcula cuantos marcos se requiere para el proceso iP
    iCantMarcos = ceil(iCantMarcos);
    int iCont = 0, iProceso;

    if (iCantMarcos > iNumMarco) {return;}
          //SWAP()}
    else { //si hay espacio en memoria no hay necesidad de reemplazo

        for(int i = 0;i<2048;i+=16){ //hace un recorrido por toda la memoria
            if(iCont>=iCantMarcos) break;
            if(MemoriaReal[i].getFree()){ //si primera posición del marco esta vacia, todo el marco esta vacio
                for(int k=0;k<16;k++){
                    MemoriaReal[i+k].setFree();
                    MemoriaReal[i+k].setProceso(iP);
                    for(int j=0;j<ListaProcesos.size();j++){//busca el proceso en la lista de procesos
                        if(ListaProcesos[j].getID()==iP) iProceso = j;
                    }
                    pair<int, bool> p; //se crea un pair con número de marco de pagina en first y en second un bool si esta o no en  memoria
                    p.first = MemoriaReal[i+k].getMarco();
                    p.second = true;
                    vB.push_back(p);
                }
                iCont++;
            }//if
        }
          ListaProcesos.push_back(Proceso(iTam,iP,vB)); //se agrega el proceso a la lista de procesos
          iNumMarco -= iCantMarcos;
    }//else



    for(int i=0;i<2048;i++){
        cout<<MemoriaReal[i].getFree()<<" "<<MemoriaReal[i].getMarco()<<" "<<MemoriaReal[i].getProceso()<<endl;
    }

    for(int i=0;i<ListaProcesos.size();i++){
        for(int k=0;k<ListaProcesos[i].vBytes.size();k++){
            cout<<"ID: "<<ListaProcesos[i].getID()<<" "<<ListaProcesos[i].vBytes[k].first<<" "<<ListaProcesos[i].vBytes[k].second<<endl;
        }
    }

    return;
}

int main()
{
       //Este for le dice a cada byte de la memoria su numero de marco de página
    for(int i=0;i<2048;i++){
        if(i<16) MemoriaReal[i].setMarcoP(0); else {
           MemoriaReal[i].setMarcoP(floor(i/16));
        }
    }//for

    char cOption;
    int iP, iNumMarco = 128;
    double iTam;

    while(cOption!='E'){
        cin>>cOption;
        cOption = toupper(cOption);

        switch(cOption){

            case 'P':// load proceso a memoria
                cin>>iTam>>iP;

                LoadInMem(iTam, iP, iNumMarco);

            break;


            case 'A': //acceso a la dir virtual de proceso

            break;

            case 'L':  //liberar memoria real


            break;

            case 'C':  //comentarios

            break;


            case 'F':  //fin dar estadísticas

            break;


        }//switch

    }//while


    return 0;
}
