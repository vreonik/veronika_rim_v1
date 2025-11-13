#include "konteineriu_testavimas.h"
#include "apdorojimas.h"
#include "nuskaityti.h"
#include "failu_generavimas.h"
#include "util.h"
#include "strategijos.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <iomanip>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::list;
using std::ofstream;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

void testuoti_strategija_su_vidurkiu(const string& fname,
                                   char b,
                                   char rikiuoti_kriterijus,
                                   int testu_kartai,
                                   ofstream& rezultatai,
                                   int strategijos_nr) {
    auto N = nuskaityti(fname).size();
    
    long long vector_skirstymo_sum = 0, list_skirstymo_sum = 0;
    size_t vector_atmintis_sum = 0, list_atmintis_sum = 0;
    
    for (int k = 0; k < testu_kartai; k++) {
        // Testuojame vector
        {
            auto visi = nuskaityti(fname);
            vector<Studentas> vargsiukai, kietakiai;
            
            TestoRezultatai rez;
            switch(strategijos_nr) {
                case 1:
                    rez = strategija_1(visi, vargsiukai, kietakiai, b);
                    break;
                case 2:
                    rez = strategija_2(visi, vargsiukai, b);
                    break;
                case 3:
                    rez = strategija_3(visi, vargsiukai, b);
                    break;
            }
            
            vector_skirstymo_sum += rez.skirstymo_laikas;
            vector_atmintis_sum += rez.atmintis_bendra;
        }
        
        // Testuojame list
        {
            auto visi = nuskaityti_i_list(fname);
            list<Studentas> vargsiukai, kietakiai;
            
            TestoRezultatai rez;
            switch(strategijos_nr) {
                case 1:
                    rez = strategija_1(visi, vargsiukai, kietakiai, b);
                    break;
                case 2:
                    rez = strategija_2(visi, vargsiukai, b);
                    break;
                case 3:
                    rez = strategija_3(visi, vargsiukai, b);
                    break;
            }
            
            list_skirstymo_sum += rez.skirstymo_laikas;
            list_atmintis_sum += rez.atmintis_bendra;
        }
    }
    
    long long vector_vidurkis = vector_skirstymo_sum / testu_kartai;
    long long list_vidurkis = list_skirstymo_sum / testu_kartai;
    size_t vector_atmintis_vid = vector_atmintis_sum / testu_kartai;
    size_t list_atmintis_vid = list_atmintis_sum / testu_kartai;
    
    cout << "Strategija " << strategijos_nr << " - Vector: " << vector_vidurkis << "ms, "
         << vector_atmintis_vid / 1024 << " KB\n";
    cout << "Strategija " << strategijos_nr << " - List:   " << list_vidurkis << "ms, "
         << list_atmintis_vid / 1024 << " KB\n";
    
    rezultatai << N << ",strategija_" << strategijos_nr << ",vector,"
               << vector_skirstymo_sum/testu_kartai << ","
               << vector_atmintis_sum/testu_kartai << "\n";
    rezultatai << N << ",strategija_" << strategijos_nr << ",list,"
               << list_skirstymo_sum/testu_kartai << ","
               << list_atmintis_sum/testu_kartai << "\n";
}

void testuoti_visas_strategijas(const string& fname) {
    char b = 'v';  // vidurkis
    int testu_kartai = 3;
    
    cout << "\n=== STRATEGIJŲ PALYGINIMAS ===\n";
    cout << "Failas: " << fname << " (" << testu_kartai << " kartų vidurkis)\n";
    
    auto temp_visi = nuskaityti(fname);
    auto N = temp_visi.size();
    cout << "Failo dydis: " << N << " įrašų\n";
    
    ofstream strategiju_rezultatai("strategiju_palyginimas.txt", std::ios::app);
    strategiju_rezultatai << "\nFailas: " << fname << "\n";
    strategiju_rezultatai << "Dydis, Strategija, Konteineris, Laikas(ms), Atmintis(baitai)\n";

    for (int strategija = 1; strategija <= 3; strategija++) {
        cout << "\n--- Strategija " << strategija << " ---\n";
        testuoti_strategija_su_vidurkiu(fname, b, 'v', testu_kartai, strategiju_rezultatai, strategija);
    }
    
    strategiju_rezultatai.close();
    cout << "\nStrategijų palyginimo rezultatai išsaugoti į 'strategiju_palyginimas.txt'\n";
}

void testuoti_konteinerius_su_failu(const string& fname) {
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimo testas su failu: " << fname << "\n";
    
    auto temp_visi = nuskaityti(fname);
    auto N = temp_visi.size();
    cout << "Failo dydis: " << N << " įrašų\n";
    
    cout << "\n--- Vector ---\n";
    apdoroti_faila<vector<Studentas>>(fname, b, rikiuoti_kriterijus);
    
    cout << "\n--- List ---\n";
    apdoroti_faila<list<Studentas>>(fname, b, rikiuoti_kriterijus);
}

void testuoti_konteinerius_sugeneruotus() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000, 10000000};
    int nd_kiek = 5;
    
    cout << "Konteinerių palyginimo testas su " << dydziai.size() << " dydžiais:\n";
    cout << "Dydžiai: ";
    for (int dydis : dydziai) {
        cout << dydis << " ";
    }
    cout << "\n";
    
    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų ===\n";
        
        string fname = "palyginimo_test_" + std::to_string(N) + ".txt";
        
        cout << "Generuojamas failas...";
        generuoti_faila(N, fname, nd_kiek);
        cout << " baigta\n";
        
        testuoti_konteinerius_su_failu(fname);
        testuoti_visas_strategijas(fname);
    }
}

void testuoti_konteinerius_su_pasirinktu_failu() {
    cout << "Įveskite failo pavadinimą konteinerių palyginimui: ";
    string fname;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(cin, fname);
    
    if (fname.empty()) {
        fname = "kursiokai.txt";
    }

    std::ifstream testas(fname);
    if (!testas.good()) {
        cout << "Klaida: Failas '" << fname << "' neegzistuoja!\n";
        cout << "Ar norite sugeneruoti naują failą? (t/n): ";
        char pasirinkimas;
        cin >> pasirinkimas;
        if (pasirinkimas == 't' || pasirinkimas == 'T') {
            cout << "Kiek įrašų generuoti? ";
            int N; cin >> N;
            cout << "Kiek ND pažymių? ";
            int nd; cin >> nd;
            generuoti_faila(N, fname, nd);
            cout << "Failas '" << fname << "' sugeneruotas.\n";
        } else {
            return;
        }
    }
    testas.close();
    
    testuoti_konteinerius_su_failu(fname);
    testuoti_visas_strategijas(fname);
}

void testuoti_strategijas_su_visais_dydziais() {
    vector<int> dydziai = {1000, 10000, 100000};
    int testu_kartai = 3;
    int nd_kiek = 5;
    
    cout << "STRATEGIJŲ PALYGINIMAS SU VISIAIS DYDŽIAIS\n";
    cout << "Testuojami dydžiai: ";
    for (int dydis : dydziai) cout << dydis << " ";
    cout << "\n";
    
    ofstream strategiju_rezultatai("strategiju_palyginimas_visi_dydziai.txt");
    strategiju_rezultatai << "Dydis, Strategija, Konteineris, Laikas(ms), Atmintis(baitai)\n";

    for (int N : dydziai) {
        cout << "\n=== Dydis: " << N << " ===\n";
        
        string fname = "strategiju_test_" + std::to_string(N) + ".txt";
        
        std::ifstream testas(fname);
        if (!testas.good()) {
            cout << "Generuojamas failas...";
            generuoti_faila(N, fname, nd_kiek);
            cout << " baigta\n";
        }
        testas.close();

        for (int strategija = 1; strategija <= 3; strategija++) {
            cout << "Strategija " << strategija << "...\n";
            
            auto temp_visi = nuskaityti(fname);
            auto actual_N = temp_visi.size();
            
            long long vector_sum = 0, list_sum = 0;
            size_t vector_atmintis = 0, list_atmintis = 0;
            
            for (int k = 0; k < testu_kartai; k++) {
                // Vector testas
                {
                    auto visi = nuskaityti(fname);
                    vector<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                        case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                        case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                    }
                    vector_sum += rez.skirstymo_laikas;
                    vector_atmintis += rez.atmintis_bendra;
                }
                
                // List testas
                {
                    auto visi = nuskaityti_i_list(fname);
                    list<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                        case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                        case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                    }
                    list_sum += rez.skirstymo_laikas;
                    list_atmintis += rez.atmintis_bendra;
                }
            }
            
            strategiju_rezultatai << actual_N << ",strategija_" << strategija << ",vector,"
                                << vector_sum/testu_kartai << "," << vector_atmintis/testu_kartai << "\n";
            strategiju_rezultatai << actual_N << ",strategija_" << strategija << ",list,"
                                << list_sum/testu_kartai << "," << list_atmintis/testu_kartai << "\n";
        }
    }
    
    strategiju_rezultatai.close();
    cout << "\nStrategijų palyginimo rezultatai išsaugoti į 'strategiju_palyginimas_visi_dydziai.txt'\n";
}

void atlikti_isamiai_analize(const string& fname, int testu_kartai) {
    cout << "\n=== IŠSAMI STRATEGIJŲ ANALIZĖ ===\n";
    cout << "Failas: " << fname << "\n";
    
    auto visi_studentai = nuskaityti(fname);
    auto N = visi_studentai.size();
    cout << "Studentų skaičius: " << N << "\n";
    
    ofstream analizes_failas("strategiju_analize_" + be_priesdelio(fname) + ".csv");
    analizes_failas << "Strategija,Konteineris,Laikas(ms),Atmintis_KB\n";
    
    for (int strategija = 1; strategija <= 3; strategija++) {
        cout << "\n--- Strategija " << strategija << " ---\n";
        
        for (int konteineris = 0; konteineris < 2; konteineris++) {
            long long total_laikas = 0;
            size_t total_atmintis = 0;
            
            for (int k = 0; k < testu_kartai; k++) {
                if (konteineris == 0) {
                    auto visi = nuskaityti(fname);
                    vector<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                        case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                        case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                    }
                    
                    total_laikas += rez.skirstymo_laikas;
                    total_atmintis += rez.atmintis_bendra;
                } else {
                    auto visi = nuskaityti_i_list(fname);
                    list<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                        case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                        case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                    }
                    
                    total_laikas += rez.skirstymo_laikas;
                    total_atmintis += rez.atmintis_bendra;
                }
            }
            
            long long vid_laikas = total_laikas / testu_kartai;
            size_t vid_atmintis = total_atmintis / testu_kartai;
            
            string konteinerio_pav = (konteineris == 0) ? "vector" : "list";
            
            cout << konteinerio_pav << ": " << vid_laikas << "ms, "
                 << vid_atmintis/1024 << " KB\n";
            
            analizes_failas << strategija << "," << konteinerio_pav << ","
                           << vid_laikas << "," << vid_atmintis/1024 << "\n";
        }
    }
    
    analizes_failas.close();
    cout << "\nIšsami analizė išsaugota į: strategiju_analize_" << be_priesdelio(fname) << ".csv\n";
}

void palyginti_strategijas_pagal_dydi() {
    vector<int> dydziai = {1000, 10000, 100000};
    int testu_kartai = 3;
    
    cout << "\n=== STRATEGIJŲ PALYGINIMAS PAGAL DYDĮ ===\n";
    
    ofstream rezultatai("strategiju_palyginimas_pagal_dydi.csv");
    rezultatai << "Dydis,Strategija,Konteineris,Laikas(ms),Atmintis_KB\n";
    
    for (int N : dydziai) {
        cout << "\nDydis: " << N << "\n";
        
        string fname = "analize_" + std::to_string(N) + ".txt";
        
        std::ifstream testas(fname);
        if (!testas.good()) {
            cout << "Generuojamas failas...";
            generuoti_faila(N, fname, 5);
            cout << " baigta\n";
        }
        testas.close();
        
        for (int strategija = 1; strategija <= 3; strategija++) {
            for (int konteineris = 0; konteineris < 2; konteineris++) {
                long long total_laikas = 0;
                size_t total_atmintis = 0;
                
                for (int k = 0; k < testu_kartai; k++) {
                    if (konteineris == 0) {
                        auto visi = nuskaityti(fname);
                        vector<Studentas> vargsiukai, kietakiai;
                        
                        TestoRezultatai rez;
                        switch(strategija) {
                            case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                            case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                            case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                        }
                        
                        total_laikas += rez.skirstymo_laikas;
                        total_atmintis += rez.atmintis_bendra;
                    } else {
                        auto visi = nuskaityti_i_list(fname);
                        list<Studentas> vargsiukai, kietakiai;
                        
                        TestoRezultatai rez;
                        switch(strategija) {
                            case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                            case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                            case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                        }
                        
                        total_laikas += rez.skirstymo_laikas;
                        total_atmintis += rez.atmintis_bendra;
                    }
                }
                
                long long vid_laikas = total_laikas / testu_kartai;
                size_t vid_atmintis = total_atmintis / testu_kartai;
                string konteinerio_pav = (konteineris == 0) ? "vector" : "list";
                
                cout << "S" << strategija << "-" << konteinerio_pav << ": "
                     << vid_laikas << "ms, " << vid_atmintis/1024 << " KB\n";
                
                rezultatai << N << "," << strategija << "," << konteinerio_pav << ","
                          << vid_laikas << "," << vid_atmintis/1024 << "\n";
            }
        }
    }
    
    rezultatai.close();
    cout << "\nRezultatai išsaugoti į: strategiju_palyginimas_pagal_dydi.csv\n";
}
