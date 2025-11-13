#include "util.h"
#include <algorithm>

std::string be_priesdelio(const std::string &kelias){
    auto pos = kelias.find_last_of("/\\");
    std::string vardas = (pos == std::string::npos) ? kelias : kelias.substr(pos + 1);
    auto taskas = vardas.find_last_of('.');
    if (taskas == std::string::npos) return vardas;
    return vardas.substr(0, taskas);
}

int gauti_vardo_numeri(const std::string &vardas){
    size_t pos = vardas.find_first_of("0123456789");
    if (pos == std::string::npos) return 0;
    return stoi(vardas.substr(pos));
}

std::pair<double,double> skaiciuoti_galutinius(const Studentas &s){
    double vid = skaiciuoti_vidurki(s.nd);
    double med = skaiciuoti_mediana(s.nd);
    return {vid * 0.4 + s.egzas * 0.6, med * 0.4 + s.egzas * 0.6};
}
