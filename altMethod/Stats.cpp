#include "Stats.h"
#include <vector>
#include <iostream>
using namespace std;
Stats::Stats()
{
  dTime=0;
  vCantSwaps = make_pair(0,0);
}

double Stats::getTime(){
    return dTime;
}

void Stats::setTime(double d){
    dTime = d;
}

void Stats::calcTA(double StartTime, int id){
double TA = dTime - StartTime;
vTA.push_back(make_pair(id,TA));



}

void Stats::setCantSwaps(int si, int so){
vCantSwaps.first = si;
vCantSwaps.second = so;
}

pair<int,int> Stats::getCantSwaps(){
return vCantSwaps;
}


void Stats::printStats(){
cout<<endl;
cout<<"--------------- Turnaround time por proceso -----------------"<<endl;
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
cout<<"---------------------------"<<endl;

cout<<vPF.size()<<endl;
for(int i=0;i<vPF.size();i++){
    cout<<"ID: "<<vPF[i].first<<" "<<"# de Page Faults: "<<vPF[i].second<<endl;
}

}

void Stats::addPF(int i){
vPF[i].second++;
}

void Stats::pushb(pair<int,int> p){
vPF.push_back(p);
}

vector<pair<int,int>> Stats::getPF(){
    return vPF;
}

void Stats::wipe(){
dTime = 0;
vTA.clear();
vPF.clear();
vCantSwaps.first = 0;
vCantSwaps.second = 0;

}
