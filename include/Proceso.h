#ifndef PROCESO_H
#define PROCESO_H
#include <utility>
#include <vector>
using namespace std;

class Proceso
{
    private:
        int iID;
        int iTam;


    public:
        Proceso(int,int, vector<pair<int,bool>>);
        int getID();
        int getiTam();
        vector<pair<int,bool>> getBytes();
        vector<pair<int,bool>> vBytes;
};

Proceso::Proceso(int tam, int id, vector<pair<int,bool>> vP){
    iTam = tam;
    iID = id;
    vBytes = vP;
}

int Proceso::getID(){
    return iID;
}

int Proceso::getiTam(){
    return iTam;
}

vector<pair<int,bool>> Proceso::getBytes(){
    return getBytes();
}





#endif // PROCESO_H
