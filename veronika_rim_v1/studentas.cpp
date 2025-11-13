#include "studentas.h"
#include <algorithm>
#include <numeric>

double skaiciuoti_vidurki(const std::vector<int>& nd){
    if(nd.empty()) return 0;
    double sum = std::accumulate(nd.begin(), nd.end(), 0.0);
    return sum / nd.size();
}

double skaiciuoti_mediana(std::vector<int> nd){
    if(nd.empty()) return 0;
    std::sort(nd.begin(), nd.end());
    size_t size = nd.size();
    if(size % 2 == 0)
        return (nd[size/2 - 1] + nd[size/2]) / 2.0;
    else
        return nd[size/2];
}
