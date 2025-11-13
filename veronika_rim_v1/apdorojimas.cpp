#include "apdorojimas.h"
#include "nuskaityti.h"
#include "util.h"
#include <algorithm>
#include <iterator>
#include <iomanip>

bool testavimo_rezimas = false;

template<typename Container>
void spausdinti_perziura(const Container& visi_stud, char pasirinkimas, int limitas) {
    if (testavimo_rezimas) return;

    std::cout << "\nPirmi " << limitas << " studentų (peržiūra):\n";
    std::cout << std::left << std::setw(15) << "Vardas"
              << std::left << std::setw(15) << "Pavarde";
    if (pasirinkimas == 'v' || pasirinkimas == 'V')
        std::cout << std::left << std::setw(20) << "Galutinis(Vid.)";
    else if (pasirinkimas == 'm' || pasirinkimas == 'M')
        std::cout << std::left << std::setw(20) << "Galutinis(Med.)";
    else
        std::cout << std::left << std::setw(20) << "Galutinis(Vid.)"
                  << std::left << std::setw(20) << "Galutinis(Med.)";

    std::cout << "\n" << std::string(70, '-') << "\n";
    int kiek = 0;
    for (const auto &s : visi_stud) {
        if (kiek++ >= limitas) break;
        auto [gvid, gmed] = skaiciuoti_galutinius(s);
        std::cout << std::left << std::setw(15) << s.vard
                  << std::left << std::setw(15) << s.pav;
        if (pasirinkimas == 'v' || pasirinkimas == 'V')
            std::cout << std::left << std::setw(20) << std::fixed << std::setprecision(2) << gvid;
        else if (pasirinkimas == 'm' || pasirinkimas == 'M')
            std::cout << std::left << std::setw(20) << std::fixed << std::setprecision(2) << gmed;
        else
            std::cout << std::left << std::setw(20) << std::fixed << std::setprecision(2) << gvid
                      << std::left << std::setw(20) << std::fixed << std::setprecision(2) << gmed;
        std::cout << "\n";
    }
    
    auto distance = std::distance(visi_stud.begin(), visi_stud.end());
    if (static_cast<size_t>(distance) > static_cast<size_t>(limitas))
        std::cout << "... ir dar " << (distance - limitas) << " įrašų.\n";
}

template<typename Container>
void klasifikuoti_ir_irasyti(const Container &visi_stud,
                             char pasirinkimas,
                             const std::string &failas_vargsiukai,
                             const std::string &failas_kietakiai,
                             long long &skirstymo_ms,
                             long long &rusiavimo_ms,
                             long long &irasymo_ms,
                             char rikiuoti_kriterijus) {
    using namespace std::chrono;
    auto pradzia = high_resolution_clock::now();

    // Sukuriame laikinus konteinerius
    std::vector<Studentas> vargsiukai, kietakiai;

    // Skirstome studentus
    for (const auto &s : visi_stud) {
        auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
        double galutinis = (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
                          (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
                          (galut_vid + galut_med) / 2.0;

        if (galutinis < 5.0)
            vargsiukai.push_back(s);
        else
            kietakiai.push_back(s);
    }

    auto skirstymo_pabaiga = high_resolution_clock::now();
    skirstymo_ms = duration_cast<milliseconds>(skirstymo_pabaiga - pradzia).count();

    auto rusiavimo_pradzia = high_resolution_clock::now();

    // Rikiuojame pagal pasirinktą kriterijų
    auto rikiuoti = [rikiuoti_kriterijus](const Studentas &a, const Studentas &b) {
        auto [a_vid, a_med] = skaiciuoti_galutinius(a);
        auto [b_vid, b_med] = skaiciuoti_galutinius(b);
        
        if (rikiuoti_kriterijus == 'v' || rikiuoti_kriterijus == 'V')
            return a_vid < b_vid;
        else if (rikiuoti_kriterijus == 'm' || rikiuoti_kriterijus == 'M')
            return a_med < b_med;
        else
            return a.vard < b.vard;
    };

    std::sort(vargsiukai.begin(), vargsiukai.end(), rikiuoti);
    std::sort(kietakiai.begin(), kietakiai.end(), rikiuoti);

    auto rusiavimo_pabaiga = high_resolution_clock::now();
    rusiavimo_ms = duration_cast<milliseconds>(rusiavimo_pabaiga - rusiavimo_pradzia).count();

    auto ras_pradzia = high_resolution_clock::now();
    
    // Įrašome į failus
    std::ofstream fv(failas_vargsiukai);
    std::ofstream fk(failas_kietakiai);
    fv << std::fixed << std::setprecision(2);
    fk << std::fixed << std::setprecision(2);

    // Header'iai
    fv << "Vardas Pavarde Galutinis\n";
    fk << "Vardas Pavarde Galutinis\n";

    for (const auto &s : vargsiukai) {
        auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
        double galutinis = (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
                          (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
                          (galut_vid + galut_med) / 2.0;
        fv << s.vard << " " << s.pav << " " << galutinis << "\n";
    }

    for (const auto &s : kietakiai) {
        auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
        double galutinis = (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
                          (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
                          (galut_vid + galut_med) / 2.0;
        fk << s.vard << " " << s.pav << " " << galutinis << "\n";
    }

    auto ras_pabaiga = high_resolution_clock::now();
    irasymo_ms = duration_cast<milliseconds>(ras_pabaiga - ras_pradzia).count();

    if (!testavimo_rezimas) {
        spausdinti_perziura(visi_stud, pasirinkimas);
    }
}

template<typename Container>
void apdoroti_faila(const std::string &fname, char budas, char rikiavimas) {
    auto rs = Laikmatis::now();
    Container visi;
    
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        visi = nuskaityti(fname);
    } else {
        visi = nuskaityti_i_list(fname);
    }
    
    auto re = Laikmatis::now();
    long long read_ms = std::chrono::duration_cast<ms>(re - rs).count();

    std::string bazinis = be_priesdelio(fname);
    long long skirstymo_ms = 0, rusiavimo_ms = 0, irasymo_ms = 0;
    
    klasifikuoti_ir_irasyti(visi, budas,
                           "vargsiukai_" + bazinis + ".txt",
                           "kietakiai_" + bazinis + ".txt",
                           skirstymo_ms, rusiavimo_ms, irasymo_ms, rikiavimas);

    std::cout << "Failas: " << fname
              << "\n  Skaitymas: " << read_ms << "ms"
              << "\n  Skirstymas: " << skirstymo_ms << "ms"
              << "\n  Rūšiavimas: " << rusiavimo_ms << "ms"
              << "\n  Įrašymas: " << irasymo_ms << "ms"
              << "\n  Viso: " << (read_ms + skirstymo_ms + rusiavimo_ms + irasymo_ms) << "ms\n";
}

// Eksplicitios specializacijos
template void spausdinti_perziura<std::vector<Studentas>>(const std::vector<Studentas>&, char, int);
template void spausdinti_perziura<std::list<Studentas>>(const std::list<Studentas>&, char, int);
template void klasifikuoti_ir_irasyti<std::vector<Studentas>>(const std::vector<Studentas>&, char, const std::string&, const std::string&, long long&, long long&, long long&, char);
template void klasifikuoti_ir_irasyti<std::list<Studentas>>(const std::list<Studentas>&, char, const std::string&, const std::string&, long long&, long long&, long long&, char);
template void apdoroti_faila<std::vector<Studentas>>(const std::string&, char, char);
template void apdoroti_faila<std::list<Studentas>>(const std::string&, char, char);
