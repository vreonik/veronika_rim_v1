#include "apdorojimas.h"
#include "nuskaityti.h"
#include "util.h"
#include <algorithm>
#include <iterator>

bool testavimo_rezimas = false;

template<typename Container>
void spausdinti_perziura(const Container& visi_stud, char pasirinkimas, int limitas) {
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
    
    auto distance = std::distance(visi_stud.begin(), visi_stud.end());
    if (static_cast<size_t>(distance) > static_cast<size_t>(limitas))
        cout << "... ir dar " << (distance - limitas) << " įrašų.\n";
}

template<typename Container>
void klasifikuoti_ir_irasyti(const Container &visi_stud,
                             char pasirinkimas,
                             const string &failas_vargsiukai,
                             const string &failas_kietakiai,
                             long long &skirstymo_ms,
                             long long &rusiavimo_ms,
                             long long &irasymo_ms,
                             char rikiuoti_kriterijus) {
    using namespace std::chrono;
    auto pradzia = high_resolution_clock::now();

    vector<pair<Studentas, pair<double,double>>> vargsiukai, kietakiai;

    auto container_size = std::distance(visi_stud.begin(), visi_stud.end());
    if (container_size > 100000) {
        vargsiukai.reserve(container_size / 2);
        kietakiai.reserve(container_size / 2);
    }

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

template void spausdinti_perziura<vector<Studentas>>(const vector<Studentas>&, char, int);
template void spausdinti_perziura<list<Studentas>>(const list<Studentas>&, char, int);
template void klasifikuoti_ir_irasyti<vector<Studentas>>(const vector<Studentas>&, char, const string&, const string&, long long&, long long&, long long&, char);
template void klasifikuoti_ir_irasyti<list<Studentas>>(const list<Studentas>&, char, const string&, const string&, long long&, long long&, long long&, char);
template void apdoroti_faila<vector<Studentas>>(const string&, char, char);
template void apdoroti_faila<list<Studentas>>(const string&, char, char);
