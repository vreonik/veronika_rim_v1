#include "nuskaityti.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Studentas> nuskaityti(const std::string& failas){
    std::vector<Studentas> visi_stud;
    std::ifstream fd(failas);
    if (!fd.is_open()) {
        std::cout<<"Nepavyko atidaryti failo:( "<<failas<<std::endl;
        return visi_stud;
    }

    std::string eil;
    while(std::getline(fd, eil)){
        if(eil.empty()) continue;
        std::stringstream ss(eil);
        Studentas stud;
        int paz;
        ss>>stud.vard>>stud.pav;
        while(ss >> paz) stud.nd.push_back(paz);
        if(!stud.nd.empty()){
            stud.egzas = stud.nd.back();
            stud.nd.pop_back();
        }
        visi_stud.push_back(stud);
    }
    return visi_stud;
}

void isskirti_ir_irasyti(const std::vector<Studentas>& visi_stud, char pasirinkimas, const std::string& failas_vargsiukai, const std::string& failas_kietakiai){
    std::ofstream vargs(failas_vargsiukai), kiet(failas_kietakiai);

    for(const auto &s: visi_stud){
        double galutinis = 0;
        if(pasirinkimas == 'v' || pasirinkimas == 'V')
            galutinis = skaiciuoti_vidurki(s.nd)*0.4 + s.egzas*0.6;
        else
            galutinis = skaiciuoti_mediana(s.nd)*0.4 + s.egzas*0.6;

        if(galutinis < 5.0)
            vargs<<s.vard<<" "<<s.pav<<" "<<galutinis<<"\n";
        else
            kiet<<s.vard<<" "<<s.pav<<" "<<galutinis<<"\n";
    }
}
