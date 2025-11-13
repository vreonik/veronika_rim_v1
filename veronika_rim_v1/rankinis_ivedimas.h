#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <limits>
#include "studentas.h"
#include "failu_generavimas.h"

template<typename Container>
void rankinis_ivedimas(Container& visi) {
    char dar = 't';
    while (dar == 't' || dar == 'T') {
        Studentas s;
        std::cout << "Vardas: ";
        std::cin >> s.vard;
        std::cout << "Pavardė: ";
        std::cin >> s.pav;
        
        std::cout << "Objekto adresas atmintyje: " << &s << std::endl;
        
        std::cout << "Generuoti (r) ar įvesti (i)? ";
        char pas;
        std::cin >> pas;

        if (pas == 'i' || pas == 'I') {
            std::cout << "Įveskite ND (tuščia eilutė - pabaiga):\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            while (true) {
                std::cout << "Pažymys: ";
                std::string eil;
                std::getline(std::cin, eil);
                if (eil.empty()) break;
                std::stringstream ss(eil);
                int nd;
                if (ss >> nd) s.nd.push_back(nd);
            }
            std::cout << "Egzamino pažymys: ";
            std::cin >> s.egzas;
        } else {
            int kiek;
            std::cout << "Kiek ND generuoti? ";
            std::cin >> kiek;
            for (int i = 0; i < kiek; i++) s.nd.push_back(gen_paz());
            s.egzas = gen_paz();
        }
        visi.push_back(s);
        
        std::cout << "Dar pridėti? (t/T): ";
        std::cin >> dar;
    }
}
