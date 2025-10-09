#include <iostream>
#include <iomanip>
#include <vector>
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

void spausdinti_perziura(const vector<Studentas>& visi_stud, char pasirinkimas, int limitas = 10) {
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
    if ((int)visi_stud.size() > limitas)
        cout << "... ir dar " << (visi_stud.size() - limitas) << " įrašų.\n";
}

void klasifikuoti_ir_irasyti(const vector<Studentas> &visi_stud,
                              char pasirinkimas,
                              const string &failas_vargsiukai,
                              const string &failas_kietakiai,
                              long long &klasifik_ms,
                              long long &irasymo_ms) {
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

    sort(vargsiukai.begin(), vargsiukai.end(), [](auto &a, auto &b){
        return gauti_vardo_numeri(a.first.vard) < gauti_vardo_numeri(b.first.vard);
    });
    sort(kietakiai.begin(), kietakiai.end(), [](auto &a, auto &b){
        return gauti_vardo_numeri(a.first.vard) < gauti_vardo_numeri(b.first.vard);
    });

    auto vidurys = high_resolution_clock::now();
    klasifik_ms = duration_cast<milliseconds>(vidurys - pradzia).count();

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


void apdoroti_faila(const string &fname, char budas) {
    auto rs = Laikmatis::now();
    vector<Studentas> visi = nuskaityti(fname);
    auto re = Laikmatis::now();
    long long read_ms = std::chrono::duration_cast<ms>(re - rs).count();

    string bazinis = be_priesdelio(fname);
    long long c = 0, w = 0;
    klasifikuoti_ir_irasyti(visi, budas, "vargsiukai_" + bazinis + ".txt",
                             "kietakiai_" + bazinis + ".txt", c, w);

    cout << "Failas: " << fname
         << "Skaitymas=" << read_ms
         << "ms, Klasifikavimas=" << c
         << "ms, Įrašymas=" << w << "ms\n";
}


int main() {
    cout << "Pasirinkite režimą:\n"
         << " f - skaityti iš failo\n"
         << " g - sugeneruoti failą ir naudoti jį\n"
         << " p - įvesti/generuoti patiems\n"
         << " t - testuoti (5 dydžiai, 5 kartai)\n"
         << " Pasirinkimas: ";

    char rez; cin >> rez;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (rez == 't' || rez == 'T') {
        testavimo_rezimas = true;

        vector<int> dydziai = {100, 1000, 10000, 100000, 1000000};
        int nd_kiek = 5;
        cout << "Testavimas 5 dydžiais, po 5 kartus.\n";
        cout << "Galutinio balo būdas (v/m): ";
        char b; cin >> b;

        ofstream rezultatai("rezultatai_testai.txt");
        rezultatai << "Dydis, Gen(ms), Skaitymas(ms), Klasifikavimas(ms), Įrašymas(ms)\n";

        for (int N : dydziai) {
            long long gen_sum = 0, read_sum = 0, klas_sum = 0, iras_sum = 0;

            for (int k = 0; k < 5; k++) {
                string fname = "test_" + std::to_string(N) + ".txt";
                auto g_s = Laikmatis::now();
                generuoti_faila(N, fname, nd_kiek);
                auto g_e = Laikmatis::now();
                gen_sum += std::chrono::duration_cast<ms>(g_e - g_s).count();

                auto r_s = Laikmatis::now();
                vector<Studentas> visi = nuskaityti(fname);
                auto r_e = Laikmatis::now();
                read_sum += std::chrono::duration_cast<ms>(r_e - r_s).count();

                long long c = 0, w = 0;
                klasifikuoti_ir_irasyti(visi, b,
                    "vargsiukai_test_" + std::to_string(N) + ".txt",
                    "kietakiai_test_" + std::to_string(N) + ".txt", c, w);
                klas_sum += c; iras_sum += w;
            }

            cout << "n=" << N << ": gen=" << gen_sum/5
                 << "ms, skaitymas=" << read_sum/5
                 << "ms, klasifikavimas=" << klas_sum/5
                 << "ms, įrašymas=" << iras_sum/5 << "ms\n";

            rezultatai << N << " | " << gen_sum/5 << " | " << read_sum/5
                        << " | " << klas_sum/5 << " | " << iras_sum/5 << "\n";
        }

        rezultatai.close();
        cout << "\nRezultatai išsaugoti į 'rezultatai_testai.txt'\n";
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
            apdoroti_faila(fname, b);
        }
        return 0;
    }

    if (rez == 'f' || rez == 'F') {
        cout << "Failo pavadinimas: ";
        string fname; getline(cin, fname);
        if (fname.empty()) fname = "kursiokai.txt";
        cout << "Balo būdas (v/m/a): ";
        char b; cin >> b;
        apdoroti_faila(fname, b);
        return 0;
    }

    if (rez == 'p' || rez == 'P') {
        vector<Studentas> visi;
        char dar = 't';
        while (dar == 't' || dar == 'T') {
            Studentas s;
            cout << "Vardas: "; cin >> s.vard;
            cout << "Pavardė: "; cin >> s.pav;
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
            cout << "Dar pridėti? (t/T): "; cin >> dar;
        }

        cout << "Balo būdas (v/m/a): ";
        char b; cin >> b;
        long long c = 0, w = 0;
        klasifikuoti_ir_irasyti(visi, b, "vargsiukai_manual.txt", "kietakiai_manual.txt", c, w);
        cout << "Išvesta. Klasifikavimas=" << c << "ms, Įrašymas=" << w << "ms\n";
        return 0;
    }

    cout << "Nesuprantamas pasirinkimas.\n";
    return 0;
}
