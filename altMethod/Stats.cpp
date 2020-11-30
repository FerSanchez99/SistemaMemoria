#include "Stats.h"
#include <vector>
#include <iostream>
using namespace std;

///////////////
//// Nombre: Stats()
//// Descripción: Constructor de la clase Stats, incializa el tiempo en 0 y la cant de swaps en 0's
//// Parametros: NO
//// Return: NO
//////////////
Stats::Stats()
{
  dTime=0;
  vCantSwaps = make_pair(0,0);
}


///////////////
//// Nombre: getTime()
//// Descripción: Return el tiempo
//// Parametros: NO
//// Return: double dTime
//////////////
double Stats::getTime(){
    return dTime;
}


///////////////
//// Nombre: setTime
//// Descripción: Cambia el valor de dTime a un valor dado como parámetro
//// Parametros: double d = nuevo valor del tiempo
//// Return: NO
//////////////
void Stats::setTime(double d){
    dTime = d;
}



///////////////
//// Nombre: calcTA
//// Descripción: Calcula el turnaround time de un proceso una vez liberado usando el dTime de ese momento y el tiempo inicial del proceso y lo agrega a vTA
//// Parametros: int id = id del proceso, double StartTime = tiempo inicial del proceso
//// Return: NO
//////////////
void Stats::calcTA(double StartTime, int id){
double TA = dTime - StartTime;
vTA.push_back(make_pair(id,TA));
}


///////////////
//// Nombre: setCantSwaps
//// Descripción: Suma el # de swaps in y swaps out al pair vCantSwaps
//// Parametros: int si = # de swaps in, int so = # de swaps out
//// Return: NO
//////////////
void Stats::setCantSwaps(int si, int so){
vCantSwaps.first = si;
vCantSwaps.second = so;
}


///////////////
//// Nombre: getCantSwaps
//// Descripción: regresa el par vCantSwaps
//// Parametros: NO
//// Return: pair<int, int> vCantSwaps
//////////////
pair<int,int> Stats::getCantSwaps(){
return vCantSwaps;
}



///////////////
//// Nombre: printStats()
//// Descripción: Imprime las estadísticas del sistema de memoria (opción F)
//// Parametros: NO
//// Return: NO
//////////////
void Stats::printStats(){
cout<<endl;
cout<<"------ Turnaround time por proceso -------"<<endl;
for(int i=0;i<vTA.size();i++){
    cout<<"ID: "<<vTA[i].first<<" Turnaround: "<<vTA[i].second<<endl;
}

cout<<"---------------------------"<<endl;
double dSum = 0;
for(int i=0;i<vTA.size();i++){
    dSum+=vTA[i].second;
}
dSum/=vTA.size();

cout<<"Turnaround Promedio: "<<dSum<<endl;

cout<<"---------------------------"<<endl;
cout<<"# Swaps in: "<<vCantSwaps.first<<endl;
cout<<"# Swaps out: "<<vCantSwaps.second<<endl;
cout<<"# Swaps total: "<<vCantSwaps.first+vCantSwaps.second<<endl;
cout<<"---------------------------"<<endl;

cout<<vPF.size()<<endl;
for(int i=0;i<vPF.size();i++){
    cout<<"ID: "<<vPF[i].first<<" "<<"# de Page Faults: "<<vPF[i].second<<endl;
}

}



///////////////
//// Nombre: addPF
//// Descripción: Le suma uno a la cantidad de Page Faults del proceso con id = i.
//// Parametros: int i = id de un proceso
//// Return: NO
//////////////
void Stats::addPF(int i){
vPF[i].second++;
}




///////////////
//// Nombre: pushb
//// Descripción: Crea un par en vPF, es decir primer page fault de cierto proceso
//// Parametros: pair<int, int> p = par nuevo donde first es id de un proceso y second = 1
//// Return: NO
//////////////
void Stats::pushb(pair<int,int> p){
vPF.push_back(p);
}



///////////////
//// Nombre: getPF()
//// Descripción: Regresa el vector vPF, #de Page Faults por proceso
//// Parametros: NO
//// Return: vector< pair<int, int>> vPF
//////////////
vector<pair<int,int>> Stats::getPF(){
    return vPF;
}



///////////////
//// Nombre: wipe()
//// Descripción: 'Resetea' todas las estadísticas del sistema de memoria
//// Parametros: NO
//// Return: NO
//////////////
void Stats::wipe(){
dTime = 0;
vTA.clear();
vPF.clear();
vCantSwaps.first = 0;
vCantSwaps.second = 0;

}
