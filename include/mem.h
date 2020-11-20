#ifndef MEM_H
#define MEM_H
/* Clase que emula una byte de memoria real */

class mem
{
    private:
        bool bFree; //checa si este byte de la mem está libre (1) o ocupado (0)
        int iProceso;   //si esta ocupado, cual es el proceso que lo ocupa
        int iMarcoP; //marco de pag al que pertenece

    public:
        mem();
        void setFree();
        void setProceso(int);
        void setMarcoP(int);
        bool getFree();
        int getProceso();
        int getMarco();
};

mem::mem(){
    bFree = true;
    iProceso = -1;
    iMarcoP = -1;
}

bool mem::getFree(){
    return bFree;
}

int mem::getMarco(){
    return iMarcoP;
}

int mem::getProceso(){
    return iProceso;
}

void mem::setFree(){
    bFree = !bFree;
}

void mem::setProceso(int iP){
    iProceso = iP;
}

void mem::setMarcoP(int iM){
    iMarcoP = iM;
}




#endif // MEM_H
