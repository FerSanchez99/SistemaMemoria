#ifndef STATS_H
#define STATS_H
#include <vector>
#include <utility>
using namespace std;
class Stats
{
    private:
        double dTime;
        vector< pair<int,double> > vTA;
        pair<int,int>  vCantSwaps;
        vector< pair<int,int> > vPF;

    public:
        Stats();
        double getTime();
        vector<pair<int,double>> getTA();
        pair<int,int> getCantSwaps();
        vector<pair<int,int>> getPF();
        void setTime(double);
        void setCantSwaps(int,int);
        void calcTA(double,int);
        void printStats();
        void pushb(pair<int,int>);
        void addPF(int);
        void wipe();





};

#endif // STATS_H
