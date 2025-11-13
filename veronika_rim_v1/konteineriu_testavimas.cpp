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
    
    long long vector_skirstymo_sum = 0, vector_rusiavimo_sum = 0;
    long long list_skirstymo_sum = 0, list_rusiavimo_sum = 0;
    size_t vector_atmintis_sum = 0, list_atmintis_sum = 0;
    
    for (int k = 0; k < testu_kartai; k++) {
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
                    (void)rikiuoti_kriterijus;
                    break;
                case 3:
                    rez = strategija_3(visi, vargsiukai, b);
                    (void)rikiuoti_kriterijus;
                    break;
            }
            
            vector_skirstymo_sum += rez.skirstymo_laikas;
            vector_rusiavimo_sum += rez.rusiavimo_laikas;
            vector_atmintis_sum += rez.atmintis_bendra;
        }
        
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
                    (void)rikiuoti_kriterijus;
                    break;
                case 3:
                    rez = strategija_3(visi, vargsiukai, b);
                    (void)rikiuoti_kriterijus;
                    break;
            }
            
            list_skirstymo_sum += rez.skirstymo_laikas;
            list_rusiavimo_sum += rez.rusiavimo_laikas;
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
               << vector_rusiavimo_sum/testu_kartai << ","
               << vector_atmintis_sum/testu_kartai << "\n";
    rezultatai << N << ",strategija_" << strategijos_nr << ",list,"
               << list_skirstymo_sum/testu_kartai << ","
               << list_rusiavimo_sum/testu_kartai << ","
               << list_atmintis_sum/testu_kartai << "\n";
}

void testuoti_visas_strategijas(const string& fname) {
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    int testu_kartai = 3;
    
    cout << "\n=== STRATEGIJŲ PALYGINIMAS ===" << endl;
    cout << "Failas: " << fname << " (" << testu_kartai << " kartų vidurkis)\n";
    
    auto temp_visi = nuskaityti(fname);
    auto N = temp_visi.size();
    cout << "Failo dydis: " << N << " įrašų\n";
    
    ofstream strategiju_rezultatai("strategiju_palyginimas.txt", std::ios::app);
    strategiju_rezultatai << "\nFailas: " << fname << "\n";
    strategiju_rezultatai << "Dydis, Strategija, Konteineris, Skirstymas(ms), Rūšiavimas(ms), Atmintis(baitai)\n";

    for (int strategija = 1; strategija <= 3; strategija++) {
        cout << "\n--- Strategija " << strategija << " ---" << endl;
        testuoti_strategija_su_vidurkiu(fname, b, rikiuoti_kriterijus, testu_kartai, strategiju_rezultatai, strategija);
    }
    
    strategiju_rezultatai.close();
    cout << "\nStrategijų palyginimo rezultatai išsaugoti į 'strategiju_palyginimas.txt'\n";
}

void testuoti_konteineri_su_vidurkiu(const string& fname, char b, char rikiuoti_kriterijus, int testu_kartai, ofstream& rezultatai) {
    auto N = nuskaityti(fname).size();
    
    long long vector_read_sum = 0, vector_skirstymo_sum = 0,
              vector_rusiavimo_sum = 0, vector_irasymo_sum = 0;
    
    for (int k = 0; k < testu_kartai; k++) {
        auto pradzia = Laikmatis::now();
        vector<Studentas> visi = nuskaityti(fname);
        auto skaitymo_pabaiga = Laikmatis::now();
        long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
        
        long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
        klasifikuoti_ir_irasyti(visi, b,
            "vargsiukai_test_" + be_priesdelio(fname) + "_vector.txt",
            "kietakiai_test_" + be_priesdelio(fname) + "_vector.txt",
            skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            
        vector_read_sum += read_ms;
        vector_skirstymo_sum += skirstymo_ms;
        vector_rusiavimo_sum += rusiavimo_ms;
        vector_irasymo_sum += irasymo_ms;
    }
    
    long long vector_viso = (vector_read_sum + vector_skirstymo_sum +
                           vector_rusiavimo_sum + vector_irasymo_sum) / testu_kartai;
    
    cout << "Vector vidurkis: " << vector_viso << "ms\n";
    rezultatai << N << ",vector," << vector_read_sum/testu_kartai << ","
               << vector_skirstymo_sum/testu_kartai << "," << vector_rusiavimo_sum/testu_kartai << ","
               << vector_irasymo_sum/testu_kartai << "," << vector_viso << "\n";

    long long list_read_sum = 0, list_skirstymo_sum = 0,
             list_rusiavimo_sum = 0, list_irasymo_sum = 0;
    
    for (int k = 0; k < testu_kartai; k++) {
        auto pradzia = Laikmatis::now();
        list<Studentas> visi = nuskaityti_i_list(fname);
        auto skaitymo_pabaiga = Laikmatis::now();
        long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
        
        long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
        klasifikuoti_ir_irasyti(visi, b,
            "vargsiukai_test_" + be_priesdelio(fname) + "_list.txt",
            "kietakiai_test_" + be_priesdelio(fname) + "_list.txt",
            skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            
        list_read_sum += read_ms;
        list_skirstymo_sum += skirstymo_ms;
        list_rusiavimo_sum += rusiavimo_ms;
        list_irasymo_sum += irasymo_ms;
    }
    
    long long list_viso = (list_read_sum + list_skirstymo_sum +
                         list_rusiavimo_sum + list_irasymo_sum) / testu_kartai;
    
    cout << "List vidurkis:   " << list_viso << "ms\n";
    rezultatai << N << ",list," << list_read_sum/testu_kartai << ","
               << list_skirstymo_sum/testu_kartai << "," << list_rusiavimo_sum/testu_kartai << ","
               << list_irasymo_sum/testu_kartai << "," << list_viso << "\n";
}

void testuoti_konteinerius_su_failu(const string& fname) {
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    int testu_kartai = 3;
    
    cout << "Konteinerių palyginimo testas su failu: " << fname << " (" << testu_kartai << " kartų vidurkis)\n";
    
    ofstream palyginimo_rezultatai("konteineriu_palyginimas_su_failu.txt", std::ios::app);
    palyginimo_rezultatai << "\nFailas: " << fname << "\n";
    palyginimo_rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    auto temp_visi = nuskaityti(fname);
    auto N = temp_visi.size();
    cout << "Failo dydis: " << N << " įrašų\n";
    
    testuoti_konteineri_su_vidurkiu(fname, b, rikiuoti_kriterijus, testu_kartai, palyginimo_rezultatai);
    
    palyginimo_rezultatai.close();
    cout << "\nPalyginimo rezultatai išsaugoti į 'konteineriu_palyginimas_su_failu.txt'\n";
}

void testuoti_konteinerius_su_vidurkiais() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000, 10000000};
    int testu_kartai = 3;
    int nd_kiek = 5;
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimas su " << testu_kartai << " kartų vidurkiu:\n";
    
    ofstream rezultatai("konteineriu_palyginimas_vidurkiai.txt");
    rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų (" << testu_kartai << " kartai) ===\n";
        
        string fname = "test_" + std::to_string(N) + ".txt";
        
        std::ifstream testas(fname);
        if (!testas.good()) {
            cout << "Generuojamas failas...";
            generuoti_faila(N, fname, nd_kiek);
            cout << " baigta\n";
        }
        testas.close();

        testuoti_konteineri_su_vidurkiu(fname, b, rikiuoti_kriterijus, testu_kartai, rezultatai);
        
        testuoti_visas_strategijas(fname);
    }
    
    rezultatai.close();
    cout << "\nRezultatai išsaugoti į 'konteineriu_palyginimas_vidurkiai.txt'\n";
}

void testuoti_konteinerius_sugeneruotus() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000, 10000000};
    int testu_kartai = 3;
    int nd_kiek = 5;
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimo testas su " << dydziai.size() << " dydžiais (" << testu_kartai << " kartų vidurkis):\n";
    cout << "Dydžiai: ";
    for (int dydis : dydziai) {
        cout << dydis << " ";
    }
    cout << "\n";
    
    ofstream palyginimo_rezultatai("konteineriu_palyginimas_sugeneruoti.txt");
    palyginimo_rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų (" << testu_kartai << " kartai) ===\n";
        
        string fname = "palyginimo_test_" + std::to_string(N) + ".txt";
        
        cout << "Generuojamas failas...";
        auto gen_pradzia = Laikmatis::now();
        generuoti_faila(N, fname, nd_kiek);
        auto gen_pabaiga = Laikmatis::now();
        cout << "Sugeneruota per " << std::chrono::duration_cast<ms>(gen_pabaiga - gen_pradzia).count() << "ms\n";
        
        testuoti_konteineri_su_vidurkiu(fname, b, rikiuoti_kriterijus, testu_kartai, palyginimo_rezultatai);
        
        testuoti_visas_strategijas(fname);
        
        if (N >= 1000000) {
            cout << "Atminties valymas...\n";
        }
    }
    
    palyginimo_rezultatai.close();
    cout << "\nPalyginimo rezultatai išsaugoti į 'konteineriu_palyginimas_sugeneruoti.txt'\n";
}

void testuoti_konteinerius_su_pasirinktu_failu() {
    cout << "Įveskite failo pavadinimą konteinerių palyginimui: ";
    string fname;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, fname);
    
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
    vector<int> dydziai = {1000, 10000, 100000, 1000000};
    int testu_kartai = 3;
    int nd_kiek = 5;
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "STRATEGIJŲ PALYGINIMAS SU VISIAIS DYDŽIAIS\n";
    cout << "Testuojami dydžiai: ";
    for (int dydis : dydziai) cout << dydis << " ";
    cout << "\n";
    
    ofstream strategiju_rezultatai("strategiju_palyginimas_visi_dydziai.txt");
    strategiju_rezultatai << "Dydis, Strategija, Konteineris, Skirstymas(ms), Rūšiavimas(ms), Atmintis(baitai)\n";

    for (int N : dydziai) {
        cout << "\n=== Dydis: " << N << " ===" << endl;
        
        string fname = "strategiju_test_" + std::to_string(N) + ".txt";
        
        std::ifstream testas(fname);
        if (!testas.good()) {
            cout << "Generuojamas failas...";
            generuoti_faila(N, fname, nd_kiek);
            cout << " baigta\n";
        }
        testas.close();

        for (int strategija = 1; strategija <= 3; strategija++) {
            cout << "Strategija " << strategija << "..." << endl;
            
            auto temp_visi = nuskaityti(fname);
            auto actual_N = temp_visi.size();
            
            long long vector_sum = 0, list_sum = 0;
            size_t vector_atmintis = 0, list_atmintis = 0;
            
            for (int k = 0; k < testu_kartai; k++) {
                {
                    auto visi = nuskaityti(fname);
                    vector<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, b); break;
                        case 2: rez = strategija_2(visi, vargsiukai, b); break;
                        case 3: rez = strategija_3(visi, vargsiukai, b); break;
                    }
                    vector_sum += rez.skirstymo_laikas;
                    vector_atmintis += rez.atmintis_bendra;
                }
                
                {
                    auto visi = nuskaityti_i_list(fname);
                    list<Studentas> vargsiukai, kietakiai;
                    
                    TestoRezultatai rez;
                    switch(strategija) {
                        case 1: rez = strategija_1(visi, vargsiukai, kietakiai, b); break;
                        case 2: rez = strategija_2(visi, vargsiukai, b); break;
                        case 3: rez = strategija_3(visi, vargsiukai, b); break;
                    }
                    list_sum += rez.skirstymo_laikas;
                    list_atmintis += rez.atmintis_bendra;
                }
            }
            
            strategiju_rezultatai << actual_N << ",strategija_" << strategija << ",vector,"
                                << vector_sum/testu_kartai << ",0," << vector_atmintis/testu_kartai << "\n";
            strategiju_rezultatai << actual_N << ",strategija_" << strategija << ",list,"
                                << list_sum/testu_kartai << ",0," << list_atmintis/testu_kartai << "\n";
        }
    }
    
    strategiju_rezultatai.close();
    cout << "\nStrategijų palyginimo rezultatai išsaugoti į 'strategiju_palyginimas_visi_dydziai.txt'\n";
}

void atlikti_isamiai_analize(const string& fname, int testu_kartai) {
    cout << "\n=== IŠSAMI STRATEGIJŲ ANALIZĖ ===\n";
    cout << "Failas: " << fname << endl;
    
    auto visi_studentai = nuskaityti(fname);
    auto N = visi_studentai.size();
    cout << "Studentų skaičius: " << N << endl;
    
    ofstream analizes_failas("strategiju_analize_" + be_priesdelio(fname) + ".csv");
    analizes_failas << "Strategija,Konteineris,Laikas(ms),Atmintis_KB,Vargsiukai,Kietakiai\n";
    
    for (int strategija = 1; strategija <= 3; strategija++) {
        cout << "\n--- Strategija " << strategija << " ---" << endl;
        
        for (int konteineris = 0; konteineris < 2; konteineris++) {
            long long total_laikas = 0;
            size_t total_atmintis = 0;
            size_t total_vargsiukai = 0;
            size_t total_kietakiai = 0;
            
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
                    total_vargsiukai += vargsiukai.size();
                    total_kietakiai += (strategija == 1) ? kietakiai.size() : visi.size();
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
                    total_vargsiukai += vargsiukai.size();
                    total_kietakiai += (strategija == 1) ? kietakiai.size() : visi.size();
                }
            }
            
            long long vid_laikas = total_laikas / testu_kartai;
            size_t vid_atmintis = total_atmintis / testu_kartai;
            size_t vid_vargsiukai = total_vargsiukai / testu_kartai;
            size_t vid_kietakiai = total_kietakiai / testu_kartai;
            
            string konteinerio_pav = (konteineris == 0) ? "vector" : "list";
            
            cout << konteinerio_pav << ": " << vid_laikas << "ms, "
                 << vid_atmintis/1024 << " KB, vargsiukai: " << vid_vargsiukai
                 << ", kietakiai: " << vid_kietakiai << endl;
            
            analizes_failas << strategija << "," << konteinerio_pav << ","
                           << vid_laikas << "," << vid_atmintis/1024 << ","
                           << vid_vargsiukai << "," << vid_kietakiai << "\n";
        }
    }
    
    analizes_failas.close();
    cout << "\nIšsami analizė išsaugota į: strategiju_analize_" << be_priesdelio(fname) << ".csv\n";
}

void palyginti_strategijas_pagal_dydi() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000};
    int testu_kartai = 3;
    
    cout << "\n=== STRATEGIJŲ PALYGINIMAS PAGAL DYDĮ ===\n";
    
    ofstream rezultatai("strategiju_palyginimas_pagal_dydi.csv");
    rezultatai << "Dydis,Strategija,Konteineris,Laikas(ms),Atmintis_KB\n";
    
    for (int N : dydziai) {
        cout << "\nDydis: " << N << endl;
        
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

void atlikti_greicio_analize() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000};
    int testu_kartai = 3;
    
    cout << "\n=== GREIČIO ANALIZĖ ===\n";
    
    ofstream greicio_failas("strategiju_greicio_analize.csv");
    greicio_failas << "Dydis,Vector_S1,Vector_S2,Vector_S3,List_S1,List_S2,List_S3\n";
    
    for (int N : dydziai) {
        cout << "Testuojama su " << N << " įrašų..." << endl;
        
        string fname = "greicio_test_" + std::to_string(N) + ".txt";
        
        std::ifstream testas(fname);
        if (!testas.good()) {
            generuoti_faila(N, fname, 5);
        }
        testas.close();
        
        vector<long long> vid_laikai(6, 0);
        
        for (int k = 0; k < testu_kartai; k++) {
            for (int strategija = 1; strategija <= 3; strategija++) {
                for (int konteineris = 0; konteineris < 2; konteineris++) {
                    int index = (strategija - 1) * 2 + konteineris;
                    
                    if (konteineris == 0) {
                        auto visi = nuskaityti(fname);
                        vector<Studentas> vargsiukai, kietakiai;
                        
                        TestoRezultatai rez;
                        switch(strategija) {
                            case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                            case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                            case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                        }
                        
                        vid_laikai[index] += rez.skirstymo_laikas;
                    } else {
                        auto visi = nuskaityti_i_list(fname);
                        list<Studentas> vargsiukai, kietakiai;
                        
                        TestoRezultatai rez;
                        switch(strategija) {
                            case 1: rez = strategija_1(visi, vargsiukai, kietakiai, 'v'); break;
                            case 2: rez = strategija_2(visi, vargsiukai, 'v'); break;
                            case 3: rez = strategija_3(visi, vargsiukai, 'v'); break;
                        }
                        
                        vid_laikai[index] += rez.skirstymo_laikas;
                    }
                }
            }
        }
        
        for (int i = 0; i < 6; i++) {
            vid_laikai[i] /= testu_kartai;
        }
        
        greicio_failas << N << ","
                      << vid_laikai[0] << "," << vid_laikai[2] << "," << vid_laikai[4] << ","
                      << vid_laikai[1] << "," << vid_laikai[3] << "," << vid_laikai[5] << "\n";
        
        cout << "Vector: S1=" << vid_laikai[0] << "ms, S2=" << vid_laikai[2]
             << "ms, S3=" << vid_laikai[4] << "ms\n";
        cout << "List:   S1=" << vid_laikai[1] << "ms, S2=" << vid_laikai[3]
             << "ms, S3=" << vid_laikai[5] << "ms\n";
    }
    
    greicio_failas.close();
    cout << "\nGreičio analizė išsaugota į: strategiju_greicio_analize.csv\n";
}
