#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>
#include <fstream>
#include <chrono>

#include "studentas.h"
#include "nuskaityti.h"
#include "failu_generavimas.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::fixed;
using std::setprecision;
using std::sort;
using std::stringstream;
using std::max;
using std::min;
using std::setw;
using std::left;
using std::right;
using std::pair;
using std::ofstream;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;


static string be_priesdelio(const string &kelias){
    auto pos = kelias.find_last_of("/\\");
    string vardas = (pos == string::npos) ? kelias : kelias.substr(pos + 1);
    auto taskas = vardas.find_last_of('.');
    if (taskas == string::npos) return vardas;
    return vardas.substr(0, taskas);
}

int gauti_vardo_numeri(const string &vardas){
    size_t pos = vardas.find_first_of("0123456789");
    if (pos == string::npos) return 0;
    return stoi(vardas.substr(pos));
}

pair<double,double> skaiciuoti_galutinius(const Studentas &s){
    double vid = skaiciuoti_vidurki(s.nd);
    double med = skaiciuoti_mediana(s.nd);
    return {vid * 0.4 + s.egzas * 0.6, med * 0.4 + s.egzas * 0.6};
}

bool testavimo_rezimas = false;

template<typename Container>
void spausdinti_perziura(const Container& visi_stud, char pasirinkimas, int limitas = 10) {
    if (testavimo_rezimas) return;

    cout << "\nPirmi " << limitas << " studentų (peržiūra):\n";
    cout << left << setw(15) << "Vardas"
         << left << setw(15) << "Pavarde";
    if (pasirinkimas == 'v' || pasirinkimas == 'V')
        cout << left << setw(20) << "Galutinis(Vid.)";
    else if (pasirinkimas == 'm' || pasirinkimas == 'M')
        cout << left << setw(20) << "Galutinis(Med.)";
    else
        cout << left << setw(20) << "Galutinis(Vid.)"
             << left << setw(20) << "Galutinis(Med.)";

    cout << "\n" << string(70, '-') << "\n";
    int kiek = 0;
    for (const auto &s : visi_stud) {
        if (kiek++ >= limitas) break;
        auto [gvid, gmed] = skaiciuoti_galutinius(s);
        cout << left << setw(15) << s.vard
             << left << setw(15) << s.pav;
        if (pasirinkimas == 'v' || pasirinkimas == 'V')
            cout << left << setw(20) << gvid;
        else if (pasirinkimas == 'm' || pasirinkimas == 'M')
            cout << left << setw(20) << gmed;
        else
            cout << left << setw(20) << gvid
                 << left << setw(20) << gmed;
        cout << "\n";
    }
    if (std::distance(visi_stud.begin(), visi_stud.end()) > limitas)
        cout << "... ir dar " << (std::distance(visi_stud.begin(), visi_stud.end()) - limitas) << " įrašų.\n";
}

template<typename Container>
void klasifikuoti_ir_irasyti(const Container &visi_stud,
                             char pasirinkimas,
                             const string &failas_vargsiukai,
                             const string &failas_kietakiai,
                             long long &skirstymo_ms,
                             long long &rusiavimo_ms,
                             long long &irasymo_ms,
                             char rikiuoti_kriterijus = 'v') {
    using namespace std::chrono;
    auto pradzia = high_resolution_clock::now();

    vector<pair<Studentas, pair<double,double>>> vargsiukai, kietakiai;

    for (const auto &s : visi_stud) {
        auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
        double galutinis = (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
                           (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
                           (galut_vid + galut_med) / 2.0;

        if (galutinis < 5.0)
            vargsiukai.push_back({s, {galut_vid, galut_med}});
        else
            kietakiai.push_back({s, {galut_vid, galut_med}});
    }

    auto skirstymo_pabaiga = high_resolution_clock::now();
    skirstymo_ms = duration_cast<milliseconds>(skirstymo_pabaiga - pradzia).count();

    auto rusiavimo_pradzia = high_resolution_clock::now();

    auto rikiuoti = [rikiuoti_kriterijus](const pair<Studentas, pair<double,double>> &a,
                                           const pair<Studentas, pair<double,double>> &b) {
        if (rikiuoti_kriterijus == 'v' || rikiuoti_kriterijus == 'V')
            return a.second.first < b.second.first;
        else if (rikiuoti_kriterijus == 'm' || rikiuoti_kriterijus == 'M')
            return a.second.second < b.second.second;
        else
            return a.first.vard < b.first.vard;
    };

    sort(vargsiukai.begin(), vargsiukai.end(), rikiuoti);
    sort(kietakiai.begin(), kietakiai.end(), rikiuoti);

    auto rusiavimo_pabaiga = high_resolution_clock::now();
    rusiavimo_ms = duration_cast<milliseconds>(rusiavimo_pabaiga - rusiavimo_pradzia).count();

    auto ras_pradzia = high_resolution_clock::now();
    
    ofstream fv(failas_vargsiukai);
    ofstream fk(failas_kietakiai);
    fv << fixed << setprecision(2);
    fk << fixed << setprecision(2);

    for (auto &p : vargsiukai) {
        if (pasirinkimas == 'v' || pasirinkimas == 'V')
            fv << p.first.vard << " " << p.first.pav << " " << p.second.first << "\n";
        else if (pasirinkimas == 'm' || pasirinkimas == 'M')
            fv << p.first.vard << " " << p.first.pav << " " << p.second.second << "\n";
        else
            fv << p.first.vard << " " << p.first.pav << " "
               << p.second.first << " " << p.second.second << "\n";
    }

    for (auto &p : kietakiai) {
        if (pasirinkimas == 'v' || pasirinkimas == 'V')
            fk << p.first.vard << " " << p.first.pav << " " << p.second.first << "\n";
        else if (pasirinkimas == 'm' || pasirinkimas == 'M')
            fk << p.first.vard << " " << p.first.pav << " " << p.second.second << "\n";
        else
            fk << p.first.vard << " " << p.first.pav << " "
               << p.second.first << " " << p.second.second << "\n";
    }

    auto ras_pabaiga = high_resolution_clock::now();
    irasymo_ms = duration_cast<milliseconds>(ras_pabaiga - ras_pradzia).count();

    spausdinti_perziura(visi_stud, pasirinkimas);
}

list<Studentas> nuskaityti_i_list(const std::string& failas) {
    auto temp_visi = nuskaityti(failas);
    list<Studentas> result;
    for (const auto& s : temp_visi) {
        result.push_back(s);
    }
    return result;
}

void testuoti_konteinerius() {
    vector<int> dydziai = {1000, 10000, 100000, 1000000};
    int nd_kiek = 5;
    char b = 'v';
    char rikiuoti_kriterijus = 'v';
    
    cout << "Konteinerių palyginimo testas su " << dydziai.size() << " dydžiais:\n";
    
    ofstream palyginimo_rezultatai("konteineriu_palyginimas.txt");
    palyginimo_rezultatai << "Dydis, Konteineris, Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

    for (int N : dydziai) {
        cout << "\n=== Testas su " << N << " įrašų ===\n";
        
        string fname = "palyginimo_test_" + std::to_string(N) + ".txt";
        generuoti_faila(N, fname, nd_kiek);
        
        {
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
    }
    
    palyginimo_rezultatai.close();
    cout << "\nPalyginimo rezultatai išsaugoti į 'konteineriu_palyginimas.txt'\n";
}

template<typename Container>
void apdoroti_faila(const string &fname, char budas, char rikiavimas) {
    auto rs = Laikmatis::now();
    Container visi;
    
    if constexpr (std::is_same_v<Container, vector<Studentas>>) {
        visi = nuskaityti(fname);
    } else {
        visi = nuskaityti_i_list(fname);
    }
    
    auto re = Laikmatis::now();
    long long read_ms = std::chrono::duration_cast<ms>(re - rs).count();

    string bazinis = be_priesdelio(fname);
    long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
    klasifikuoti_ir_irasyti(visi, budas, "vargsiukai_" + bazinis + ".txt",
                             "kietakiai_" + bazinis + ".txt",
                             skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiavimas);

    cout << "Failas: " << fname
         << "\n  Skaitymas: " << read_ms << "ms"
         << "\n  Skirstymas: " << skirstymo_ms << "ms"
         << "\n  Rūšiavimas: " << rusiavimo_ms << "ms"
         << "\n  Įrašymas: " << irasymo_ms << "ms"
         << "\n  Viso: " << (read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms) << "ms\n";
}

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
            while (true) {
                cout << "Pažymys: ";
                string eil; cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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


int main() {
    cout << "Pasirinkite režimą:\n"
         << " f - skaityti iš failo\n"
         << " g - sugeneruoti failą ir naudoti jį\n"
         << " p - įvesti/generuoti patiems\n"
         << " t - testuoti (5 dydžiai, 5 kartai)\n"
         << " c - konteinerių palyginimas (vector vs list)\n"
         << " Pasirinkimas: ";

    char rez; cin >> rez;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (rez == 'c' || rez == 'C') {
        testavimo_rezimas = true;
        testuoti_konteinerius();
        return 0;
    }

    cout << "Pasirinkite konteinerio tipą:\n"
         << " v - std::vector\n"
         << " l - std::list\n"
         << " Pasirinkimas: ";
    char konteinerio_tipas; cin >> konteinerio_tipas;

    cout << "Rikiuoti pagal:\n"
         << " v - vidurkį\n"
         << " m - medianą\n"
         << " p - vardą\n"
         << " Pasirinkimas: ";
    char rikiuoti_kriterijus; cin >> rikiuoti_kriterijus;

    if (rez == 't' || rez == 'T') {
        testavimo_rezimas = true;

        vector<int> dydziai = {100, 1000, 10000, 100000, 1000000};
        int nd_kiek = 5;
        cout << "Testavimas 5 dydžiais, po 5 kartus.\n";
        cout << "Galutinio balo būdas (v/m): ";
        char b; cin >> b;

        string konteinerio_pavadinimas = (konteinerio_tipas == 'v' ? "vector" : "list");
        ofstream rezultatai("rezultatai_testai_" + konteinerio_pavadinimas + ".txt");
        rezultatai << "Konteineris: " << konteinerio_pavadinimas << "\n";
        rezultatai << "Dydis, Gen(ms), Skaitymas(ms), Skirstymas(ms), Rūšiavimas(ms), Įrašymas(ms), Viso(ms)\n";

        for (int N : dydziai) {
            long long gen_sum = 0, read_sum = 0, skirstymo_sum = 0, rusiavimo_sum = 0, irasymo_sum = 0;

            for (int k = 0; k < 5; k++) {
                string fname = "test_" + std::to_string(N) + ".txt";
                auto g_s = Laikmatis::now();
                generuoti_faila(N, fname, nd_kiek);
                auto g_e = Laikmatis::now();
                gen_sum += std::chrono::duration_cast<ms>(g_e - g_s).count();

                auto r_s = Laikmatis::now();
                
                if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
                    vector<Studentas> visi = nuskaityti(fname);
                    auto r_e = Laikmatis::now();
                    read_sum += std::chrono::duration_cast<ms>(r_e - r_s).count();

                    long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
                    klasifikuoti_ir_irasyti(visi, b,
                        "vargsiukai_test_" + std::to_string(N) + "_vector.txt",
                        "kietakiai_test_" + std::to_string(N) + "_vector.txt",
                        skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
                    skirstymo_sum += skirstymo_ms;
                    rusiavimo_sum += rusiavimo_ms;
                    irasymo_sum += irasymo_ms;
                } else {
                    list<Studentas> visi = nuskaityti_i_list(fname);
                    auto r_e = Laikmatis::now();
                    read_sum += std::chrono::duration_cast<ms>(r_e - r_s).count();

                    long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
                    klasifikuoti_ir_irasyti(visi, b,
                        "vargsiukai_test_" + std::to_string(N) + "_list.txt",
                        "kietakiai_test_" + std::to_string(N) + "_list.txt",
                        skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
                    skirstymo_sum += skirstymo_ms;
                    rusiavimo_sum += rusiavimo_ms;
                    irasymo_sum += irasymo_ms;
                }
            }

            long long viso_sum = read_sum + skirstymo_sum + rusiavimo_sum + irasymo_sum;
            
            cout << "n=" << N << ":\n"
                 << "  gen=" << gen_sum/5 << "ms"
                 << ", skaitymas=" << read_sum/5 << "ms"
                 << ", skirstymas=" << skirstymo_sum/5 << "ms"
                 << ", rūšiavimas=" << rusiavimo_sum/5 << "ms"
                 << ", įrašymas=" << irasymo_sum/5 << "ms"
                 << ", viso=" << viso_sum/5 << "ms\n";

            rezultatai << N << " " << gen_sum/5 << " " << read_sum/5
                        << " " << skirstymo_sum/5 << " " << rusiavimo_sum/5
                        << " " << irasymo_sum/5 << " " << viso_sum/5 << "\n";
        }

        rezultatai.close();
        cout << "\nRezultatai išsaugoti į 'rezultatai_testai_" << konteinerio_pavadinimas << ".txt'\n";
        return 0;
    }
    if (rez == 'g' || rez == 'G') {
        cout << "Failo pavadinimas: ";
        string fname; getline(cin, fname);
        if (fname.empty()) fname = "kursiokai.txt";
        cout << "Kiek įrašų generuoti? "; int N; cin >> N;
        cout << "Kiek ND pažymių? "; int nd; cin >> nd;

        auto g_s = Laikmatis::now();
        generuoti_faila(N, fname, nd);
        auto g_e = Laikmatis::now();
        cout << "Sugeneruota per " << std::chrono::duration_cast<ms>(g_e - g_s).count() << " ms\n";

        cout << "Naudoti šį failą dabar? (t/T): ";
        char d; cin >> d;
        if (d == 't' || d == 'T') {
            cout << "Balo būdas (v/m/a): ";
            char b; cin >> b;
            if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
                apdoroti_faila<vector<Studentas>>(fname, b, rikiuoti_kriterijus);
            } else {
                apdoroti_faila<list<Studentas>>(fname, b, rikiuoti_kriterijus);
            }
        }
        return 0;
    }

    if (rez == 'f' || rez == 'F') {
        cout << "Failo pavadinimas: ";
        string fname; getline(cin, fname);
        if (fname.empty()) fname = "kursiokai.txt";
        cout << "Balo būdas (v/m/a): ";
        char b; cin >> b;
        if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
            apdoroti_faila<vector<Studentas>>(fname, b, rikiuoti_kriterijus);
        } else {
            apdoroti_faila<list<Studentas>>(fname, b, rikiuoti_kriterijus);
        }
        return 0;
    }

    if (rez == 'p' || rez == 'P') {
        if (konteinerio_tipas == 'v' || konteinerio_tipas == 'V') {
            vector<Studentas> visi;
            rankinis_ivedimas(visi);
            
            cout << "Balo būdas (v/m/a): ";
            char b; cin >> b;
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(visi, b, "vargsiukai_rankinis_vector.txt",
                                   "kietakiai_rankinis_vector.txt",
                                   skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            cout << "Išvesta. Skirstymas=" << skirstymo_ms << "ms, Rūšiavimas=" << rusiavimo_ms
                 << "ms, Įrašymas=" << irasymo_ms << "ms\n";
        } else {
            list<Studentas> visi;
            rankinis_ivedimas(visi);
            
            cout << "Balo būdas (v/m/a): ";
            char b; cin >> b;
            long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
            klasifikuoti_ir_irasyti(visi, b, "vargsiukai_rankinis_list.txt",
                                   "kietakiai_rankinis_list.txt",
                                   skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiuoti_kriterijus);
            cout << "Išvesta. Skirstymas=" << skirstymo_ms << "ms, Rūšiavimas=" << rusiavimo_ms
                 << "ms, Įrašymas=" << irasymo_ms << "ms\n";
        }
        return 0;
    }

    cout << " Nesuprantamas pasirinkimas.\n";
    return 0;
}
