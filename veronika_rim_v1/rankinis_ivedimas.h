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
        cout << "Vardas: "; cin >> s.vard;
        cout << "Pavardė: "; cin >> s.pav;
        
        cout << "Objekto adresas atmintyje: " << &s << endl;
        
        cout << "Generuoti (r) ar įvesti (i)? ";
        char pas; cin >> pas;

        if (pas == 'i' || pas == 'I') {
            cout << "Įveskite ND (tuščia eilutė - pabaiga):\n";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            while (true) {
                cout << "Pažymys: ";
                string eil;
                getline(cin, eil);
                if (eil.empty()) break;
                stringstream ss(eil);
                int nd; if (ss >> nd) s.nd.push_back(nd);
            }
            cout << "Egzamino pažymys: "; cin >> s.egzas;
        } else {
            int kiek; cout << "Kiek ND generuoti? "; cin >> kiek;
            for (int i = 0; i < kiek; i++) s.nd.push_back(gen_paz());
            s.egzas = gen_paz();
        }
        visi.push_back(s);
        
        if constexpr (std::is_same_v<Container, vector<Studentas>>) {
            cout << "Elemento adresas vektoriuje: " << &visi.back() << endl;
        } else {
            cout << "Paskutinio elemento adresas liste: " << &visi.back() << endl;
        }
        
        cout << "Dar pridėti? (t/T): "; cin >> dar;
    }
}
