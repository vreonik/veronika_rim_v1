#include "studentas.h"
#include <algorithm>

double skaiciuoti_vidurki(const vector<int>& nd){
    if(nd.empty()) return 0;
    double sum=0;
    for(int x: nd) sum += x;
    return sum/nd.size();
}

double skaiciuoti_mediana(vector<int> nd){
    if(nd.empty()) return 0;
    sort(nd.begin(), nd.end());
    if(nd.size()%2==0)
        return (nd[nd.size()/2 -1] + nd[nd.size()/2])/2.0;
    else
        return nd[nd.size()/2];
}
