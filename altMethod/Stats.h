#ifndef STATS_H
#define STATS_H
#include <vector>
#include <utility>
using namespace std;
class Stats
{
    private:
        double dTime; //Var llevará conteo del tiempo de la simulación
        vector< pair<int,double> > vTA;  //vector de pares que guarda el turnaoround de cada proceso, el pair.first esta el id proceso y en pair.second está el turnarounf
        pair<int,int>  vCantSwaps; //vector de pares que lleva conteo de swaps, pair.first es el # swaps in y en pair.second es el # swaps out
        vector< pair<int,int> > vPF; //vector de pares guarda el # de pages fault por proceso, pair.first es el id del proceso y pair.second es el # de page faults

    public:
        Stats();  //constructor
        double getTime(); //regresa el tiempo
        vector<pair<int,double>> getTA(); //regresa el vector vTA
        pair<int,int> getCantSwaps(); //regresa el par vCantSwaps
        vector<pair<int,int>> getPF(); //regresa el vector vPF
        void setTime(double);  //modifica el atributo dTime
        void setCantSwaps(int,int); //modifica el par vCantSwaps
        void calcTA(double,int); //calcula el turnaound time de cierto proceso
        void printStats(); //imprime las estadísticas
        void pushb(pair<int,int>); //hace push_back al vector vPF
        void addPF(int); //le suma uno al # de page faults de un proceso
        void wipe(); //borra todas las estadísticas





};

#endif // STATS_H
