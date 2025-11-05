#include "konteineriu_testavimas.h"
#include "apdorojimas.h"
#include "nuskaityti.h"
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

void testuoti_konteinerius_su_failu(const string& fname) {
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimo testas su failu: " << fname << "\n";
    
    ofstream palyginimo_rezultatai("konteineriu_palyginimas_su_failu.txt", std::ios::app);
    palyginimo_rezultatai << "\nFailas: " << fname << "\n";
    palyginimo_rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    auto temp_visi = nuskaityti(fname);
    auto N = temp_visi.size(); // pakeista į auto
    cout << "Failo dydis: " << N << " įrašų\n";
    
    {
        auto pradzia = Laikmatis::now();
        vector<Studentas> visi = nuskaityti(fname);
        auto skaitymo_pabaiga = Laikmatis::now();
        long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
        
        long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
        klasifikuoti_ir_irasyti(visi, b,
            "vargsiukai_palyginimo_" + be_priesdelio(fname) + "_vector.txt",
            "kietakiai_palyginimo_" + be_priesdelio(fname) + "_vector.txt",
            skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            
        long long viso_ms = read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms;
        
        cout << "Vector: " << viso_ms << "ms (skaitymas: " << read_ms
             << "ms, skirstymas: " << skirstymo_ms << "ms, rūšiavimas: " << rusiavimo_ms
             << "ms, įrašymas: " << irasymo_ms << "ms)\n";
             
        palyginimo_rezultatai << N << ",vector," << read_ms << "," << skirstymo_ms
                             << "," << rusiavimo_ms << "," << irasymo_ms << "," << viso_ms << "\n";
    }
    
    {
        auto pradzia = Laikmatis::now();
        list<Studentas> visi = nuskaityti_i_list(fname);
        auto skaitymo_pabaiga = Laikmatis::now();
        long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
        
        long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
        klasifikuoti_ir_irasyti(visi, b,
            "vargsiukai_palyginimo_" + be_priesdelio(fname) + "_list.txt",
            "kietakiai_palyginimo_" + be_priesdelio(fname) + "_list.txt",
            skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            
        long long viso_ms = read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms;
        
        cout << "List:   " << viso_ms << "ms (skaitymas: " << read_ms
             << "ms, skirstymas: " << skirstymo_ms << "ms, rūšiavimas: " << rusiavimo_ms
             << "ms, įrašymas: " << irasymo_ms << "ms)\n";
             
        palyginimo_rezultatai << N << ",list," << read_ms << "," << skirstymo_ms
                             << "," << rusiavimo_ms << "," << irasymo_ms << "," << viso_ms << "\n";
    }
    
    palyginimo_rezultatai.close();
    cout << "\nPalyginimo rezultatai išsaugoti į 'konteineriu_palyginimas_su_failu.txt'\n";
}

void testuoti_konteinerius_sugeneruotus() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000, 10000000};
    int nd_kiek = 5;
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimo testas su " << dydziai.size() << " dydžiais:\n";
    cout << "Dydžiai: ";
    for (int dydis : dydziai) {
        cout << dydis << " ";
    }
    cout << "\n";
    
    ofstream palyginimo_rezultatai("konteineriu_palyginimas_sugeneruoti.txt");
    palyginimo_rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų\n";
        
        string fname = "palyginimo_test_" + std::to_string(N) + ".txt";
        
        cout << "Generuojamas failas...";
        auto gen_pradzia = Laikmatis::now();
        generuoti_faila(N, fname, nd_kiek);
        auto gen_pabaiga = Laikmatis::now();
        cout << "Sugeneruota per " << std::chrono::duration_cast<ms>(gen_pabaiga - gen_pradzia).count() << "ms\n";
        
        {
            cout << "Testuojamas vector... ";
            auto pradzia = Laikmatis::now();
            vector<Studentas> visi = nuskaityti(fname);
            auto skaitymo_pabaiga = Laikmatis::now();
            long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
            
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(visi, b,
                "vargsiukai_palyginimo_" + std::to_string(N) + "_vector.txt",
                "kietakiai_palyginimo_" + std::to_string(N) + "_vector.txt",
                skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
                
            long long viso_ms = read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms;
            
            cout << "Vector: " << viso_ms << "ms (skaitymas: " << read_ms
                 << "ms, skirstymas: " << skirstymo_ms << "ms, rūšiavimas: " << rusiavimo_ms
                 << "ms, įrašymas: " << irasymo_ms << "ms)\n";
                 
            palyginimo_rezultatai << N << ",vector," << read_ms << "," << skirstymo_ms
                                 << "," << rusiavimo_ms << "," << irasymo_ms << "," << viso_ms << "\n";
        }
        
        {
            cout << "Testuojamas list... ";
            auto pradzia = Laikmatis::now();
            list<Studentas> visi = nuskaityti_i_list(fname);
            auto skaitymo_pabaiga = Laikmatis::now();
            long long read_ms = std::chrono::duration_cast<ms>(skaitymo_pabaiga - pradzia).count();
            
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(visi, b,
                "vargsiukai_palyginimo_" + std::to_string(N) + "_list.txt",
                "kietakiai_palyginimo_" + std::to_string(N) + "_list.txt",
                skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
                
            long long viso_ms = read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms;
            
            cout << "List:   " << viso_ms << "ms (skaitymas: " << read_ms
                 << "ms, skirstymas: " << skirstymo_ms << "ms, rūšiavimas: " << rusiavimo_ms
                 << "ms, įrašymas: " << irasymo_ms << "ms)\n";
                 
            palyginimo_rezultatai << N << ",list," << read_ms << "," << skirstymo_ms
                                 << "," << rusiavimo_ms << "," << irasymo_ms << "," << viso_ms << "\n";
        }
        
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
}
