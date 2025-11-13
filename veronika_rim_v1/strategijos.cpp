#include "strategijos.h"
#include <algorithm>
#include <chrono>
#include <iterator>

using namespace std::chrono;

// Pagalbinė funkcija galutinio pažymio skaičiavimui
double skaiciuoti_galutini_pazymi(const Studentas& s, char pasirinkimas) {
    auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
    return (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
           (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
           (galut_vid + galut_med) / 2.0;
}

// 1 STRATEGIJA - Du nauji konteineriai (dabartinė implementacija)
template<typename Container>
TestoRezultatai strategija_1(const Container& visi_stud,
                            Container& vargsiukai,
                            Container& kietakiai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    for (const auto &s : visi_stud) {
        double galutinis = skaiciuoti_galutini_pazymi(s, pasirinkimas);
        
        if (galutinis < 5.0)
            vargsiukai.push_back(s);
        else
            kietakiai.push_back(s);
    }
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0; // Rūšiavimas atliekamas atskirai
    return rez;
}

// 2 STRATEGIJA - Vienas naujas konteineris + trynimas
template<typename Container>
TestoRezultatai strategija_2(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    // Naudojame iteratorius saugiam trynimui
    auto it = visi_stud.begin();
    while (it != visi_stud.end()) {
        double galutinis = skaiciuoti_galutini_pazymi(*it, pasirinkimas);
        
        if (galutinis < 5.0) {
            // Perkeliame į vargsiukus
            vargsiukai.push_back(*it);
            // Ištriname iš pradinio konteinerio
            it = visi_stud.erase(it);
        } else {
            // Paliekame kietakius pradiniame konteineryje
            ++it;
        }
    }
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0;
    return rez;
}

// Specializuota 2 strategijos versija list'ui (efektyvesnė su splice)
template<>
TestoRezultatai strategija_2(std::list<Studentas>& visi_stud,
                            std::list<Studentas>& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    auto it = visi_stud.begin();
    while (it != visi_stud.end()) {
        double galutinis = skaiciuoti_galutini_pazymi(*it, pasirinkimas);
        
        if (galutinis < 5.0) {
            // Perkeliame elementą naudodami splice (efektyvu list'ams)
            auto next_it = std::next(it);
            vargsiukai.splice(vargsiukai.end(), visi_stud, it);
            it = next_it;
        } else {
            ++it;
        }
    }
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0;
    return rez;
}

// 3 strategija - dar neimplementuota (bus kitame commite)
template<typename Container>
TestoRezultatai strategija_3(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas) {
    // Laikina implementacija - identiška 2 strategijai
    return strategija_2(visi_stud, vargsiukai, pasirinkimas);
}

// Eksplicitios instancijos
template TestoRezultatai strategija_1<std::vector<Studentas>>(
    const std::vector<Studentas>&, std::vector<Studentas>&, std::vector<Studentas>&, char);
template TestoRezultatai strategija_1<std::list<Studentas>>(
    const std::list<Studentas>&, std::list<Studentas>&, std::list<Studentas>&, char);

template TestoRezultatai strategija_2<std::vector<Studentas>>(
    std::vector<Studentas>&, std::vector<Studentas>&, char);
template TestoRezultatai strategija_2<std::list<Studentas>>(
    std::list<Studentas>&, std::list<Studentas>&, char);

template TestoRezultatai strategija_3<std::vector<Studentas>>(
    std::vector<Studentas>&, std::vector<Studentas>&, char);
template TestoRezultatai strategija_3<std::list<Studentas>>(
    std::list<Studentas>&, std::list<Studentas>&, char);
