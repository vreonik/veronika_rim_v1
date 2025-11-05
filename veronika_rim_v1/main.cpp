#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <chrono>
#include <limits>

#include "studentas.h"
#include "nuskaityti.h"
#include "failu_generavimas.h"
#include "rankinis_ivedimas.h"
#include "konteineriu_testavimas.h"
#include "apdorojimas.h"
#include "util.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::list;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

void testuoti_programa();

int main() {
    cout << "Pasirinkite režimą:\n"
         << " f - skaityti iš failo\n"
         << " g - sugeneruoti failą ir naudoti jį\n"
         << " p - įvesti/generuoti patiems\n"
         << " t - testuoti (5 dydžiai, 5 kartai)\n"
         << " c - konteinerių palyginimas (vector vs list)\n"
         << " Pasirinkimas: ";

    char rez;
    cin >> rez;

    if (rez == 'c' || rez == 'C') {
        testavimo_rezimas = true;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        cout << "Konteinerių palyginimo pasirinkimas:\n"
             << " s - sugeneruoti naujus failus\n"
             << " e - naudoti esamą failą\n"
             << " p - pasirinkti failą patiems\n"
             << " v - testuoti su vidurkiais (3 kartai)\n"
             << " Pasirinkimas: ";
        char palyginimo_tipas;
        cin >> palyginimo_tipas;
        
        if (palyginimo_tipas == 'e' || palyginimo_tipas == 'E') {
            testuoti_konteinerius_sugeneruotus();
        } else if (palyginimo_tipas == 'p' || palyginimo_tipas == 'P') {
            testuoti_konteinerius_su_pasirinktu_failu();
        } else if (palyginimo_tipas == 'v' || palyginimo_tipas == 'V') {
            testuoti_konteinerius_su_vidurkiais();
        } else {
            testuoti_konteinerius_sugeneruotus();
        }
        return 0;
    }

    cout << "Pasirinkite konteinerio tipą:\n"
         << " v - std::vector\n"
         << " l - std::list\n"
         << " Pasirinkimas: ";
    char konteinerio_tipas;
    cin >> konteinerio_tipas;

    cout << "Rikiuoti pagal:\n"
         << " v - vidurkį\n"
         << " m - medianą\n"
         << " p - vardą\n"
         << " Pasirinkimas: ";
    char rikiuoti_kriterijus;
    cin >> rikiuoti_kriterijus;

    if (rez == 't' || rez == 'T') {
        testavimo_rezimas = true;
        testuoti_programa();
        return 0;
    }

    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (rez == 'g' || rez == 'G') {
        cout << "Failo pavadinimas: ";
        string fname;
        getline(cin, fname);
        if (fname.empty()) fname = "kursiokai.txt";
        
        cout << "Kiek įrašų generuoti? ";
        int N;
        cin >> N;
        
        cout << "Kiek ND pažymių? ";
        int nd;
        cin >> nd;

        auto g_s = Laikmatis::now();
        generuoti_faila(N, fname, nd);
        auto g_e = Laikmatis::now();
        cout << "Sugeneruota per " << std::chrono::duration_cast<ms>(g_e - g_s).count() << "ms\n";
        
        cout << "Naudoti šį failą dabar? (t/T): ";
        char d;
        cin >> d;
        
        if (d == 't' || d == 'T') {
            cout << "Pasirinkite galutinio pažymio skaičiavimo būdą:\n"
                 << " v - vidurkis\n"
                 << " m - mediana\n"
                 << " a - abu\n"
                 << " Pasirinkimas: ";
            char budas;
            cin >> budas;

            if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
                apdoroti_faila<vector<Studentas>>(fname, budas, rikiuoti_kriterijus);
            } else {
                apdoroti_faila<list<Studentas>>(fname, budas, rikiuoti_kriterijus);
            }
        }
        return 0;
    }

    if (rez == 'f' || rez == 'F') {
        cout << "Failo pavadinimas: ";
        string fname;
        getline(cin, fname);
        if (fname.empty()) fname = "kursiokai.txt";

        cout << "Pasirinkite galutinio pažymio skaičiavimo būdą:\n"
             << " v - vidurkis\n"
             << " m - mediana\n"
             << " a - abu\n"
             << " Pasirinkimas: ";
        char budas;
        cin >> budas;

        if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
            apdoroti_faila<vector<Studentas>>(fname, budas, rikiuoti_kriterijus);
        } else {
            apdoroti_faila<list<Studentas>>(fname, budas, rikiuoti_kriterijus);
        }
        return 0;
    }

    if (rez == 'p' || rez == 'P') {
        cout << "Pasirinkite galutinio pažymio skaičiavimo būdą:\n"
             << " v - vidurkis\n"
             << " m - mediana\n"
             << " a - abu\n"
             << " Pasirinkimas: ";
        char budas;
        cin >> budas;

        if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
            vector<Studentas> studentai;
            rankinis_ivedimas(studentai);
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(studentai, budas, "vargsiukai_rankiniai.txt",
                                     "kietakiai_rankiniai.txt", skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
        } else {
            list<Studentas> studentai;
            rankinis_ivedimas(studentai);
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(studentai, budas, "vargsiukai_rankiniai.txt",
                                     "kietakiai_rankiniai.txt", skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
        }
        return 0;
    }

    cout << "Nesuprantamas pasirinkimas.\n";
    return 0;
}

void testuoti_programa() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000, 10000000};
    int nd_kiek = 5;

    cout << "Testavimas su " << dydziai.size() << " dydžiais:\n";
    cout << "Dydžiai: ";
    for (int dydis : dydziai) {
        cout << dydis << " ";
    }
    cout << "\n";

    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų\n";
        string fname = "test_" + std::to_string(N) + ".txt";

        cout << "Generuojamas failas...";
        auto gen_pradzia = Laikmatis::now();
        generuoti_faila(N, fname, nd_kiek);
        auto gen_pabaiga = Laikmatis::now();
        cout << "Sugeneruota per " << std::chrono::duration_cast<ms>(gen_pabaiga - gen_pradzia).count() << "ms\n";

        cout << "Testuojamas vector... ";
        apdoroti_faila<vector<Studentas>>(fname, 'v', 'v');

        cout << "Testuojamas list... ";
        apdoroti_faila<list<Studentas>>(fname, 'v', 'v');

        if (N >= 1000000) {
            cout << "Atminties valymas...\n";
        }
    }
}
