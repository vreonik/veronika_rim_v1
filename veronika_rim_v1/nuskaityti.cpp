#include "nuskaityti.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Studentas> nuskaityti(const std::string& failas){
    std::ifstream fd(failas);
    if (!fd.is_open()) {
        std::cerr << "Nepavyko atidaryti failo: " << failas << std::endl;
        return {};
    }

    std::vector<Studentas> visi;
    std::string eil;
    std::getline(fd, eil);

    while (std::getline(fd, eil)) {
        if (eil.empty()) continue;
        std::stringstream ss(eil);
        Studentas s;
        ss >> s.vard >> s.pav;
        int paz;
        while (ss >> paz)
            s.nd.push_back(paz);

        if (!s.nd.empty()) {
            s.egzas = s.nd.back();
            s.nd.pop_back();
        } else {
            s.egzas = 0;
        }

        visi.push_back(s);
    }

    return visi;
}
