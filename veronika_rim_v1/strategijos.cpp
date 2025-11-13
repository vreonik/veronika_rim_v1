#include "strategijos.h"
#include "util.h"
#include <algorithm>
#include <chrono>
#include <iterator>

using namespace std::chrono;

double skaiciuoti_galutini_pazymi(const Studentas& s, char pasirinkimas) {
    auto [galut_vid, galut_med] = skaiciuoti_galutinius(s);
    return (pasirinkimas == 'v' || pasirinkimas == 'V') ? galut_vid :
           (pasirinkimas == 'm' || pasirinkimas == 'M') ? galut_med :
           (galut_vid + galut_med) / 2.0;
}

template<typename Container>
size_t apskaiciuoti_atminti(const Container& container) {
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        return container.capacity() * sizeof(Studentas);
    } else {
        return container.size() * sizeof(Studentas);
    }
}

template<typename Container>
TestoRezultatai strategija_1(const Container& visi_stud,
                            Container& vargsiukai,
                            Container& kietakiai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        vargsiukai.reserve(visi_stud.size() / 2);
        kietakiai.reserve(visi_stud.size() / 2);
    }
    
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
    rez.rusiavimo_laikas = 0;
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(kietakiai);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template<typename Container>
TestoRezultatai strategija_2(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        vargsiukai.reserve(visi_stud.size() / 2);
    }
    
    auto it = visi_stud.begin();
    while (it != visi_stud.end()) {
        double galutinis = skaiciuoti_galutini_pazymi(*it, pasirinkimas);
        
        if (galutinis < 5.0) {
            vargsiukai.push_back(std::move(*it));
            it = visi_stud.erase(it);
        } else {
            ++it;
        }
    }
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0;
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(visi_stud);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template<>
TestoRezultatai strategija_2(std::list<Studentas>& visi_stud,
                            std::list<Studentas>& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    auto it = visi_stud.begin();
    while (it != visi_stud.end()) {
        double galutinis = skaiciuoti_galutini_pazymi(*it, pasirinkimas);
        
        if (galutinis < 5.0) {
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
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(visi_stud);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template<typename Container>
TestoRezultatai strategija_3(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        vargsiukai.reserve(visi_stud.size() / 2);
    }
    
    auto is_vargsiukas = [pasirinkimas](const Studentas& s) {
        double galutinis = skaiciuoti_galutini_pazymi(s, pasirinkimas);
        return galutinis < 5.0;
    };
    
    std::remove_copy_if(visi_stud.begin(), visi_stud.end(),
                       std::back_inserter(vargsiukai),
                       [&is_vargsiukas](const Studentas& s) {
                           return !is_vargsiukas(s);
                       });
    
    auto new_end = std::remove_if(visi_stud.begin(), visi_stud.end(), is_vargsiukas);
    visi_stud.erase(new_end, visi_stud.end());
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0;
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(visi_stud);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template<>
TestoRezultatai strategija_3(std::list<Studentas>& visi_stud,
                            std::list<Studentas>& vargsiukai,
                            char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    auto is_vargsiukas = [pasirinkimas](const Studentas& s) {
        double galutinis = skaiciuoti_galutini_pazymi(s, pasirinkimas);
        return galutinis < 5.0;
    };
    
    auto it = visi_stud.begin();
    while (it != visi_stud.end()) {
        if (is_vargsiukas(*it)) {
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
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(visi_stud);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template<typename Container>
TestoRezultatai strategija_3_partition(Container& visi_stud,
                                      Container& vargsiukai,
                                      char pasirinkimas) {
    auto start = high_resolution_clock::now();
    
    if constexpr (std::is_same_v<Container, std::vector<Studentas>>) {
        vargsiukai.reserve(visi_stud.size() / 2);
    }
    
    auto is_kietakas = [pasirinkimas](const Studentas& s) {
        double galutinis = skaiciuoti_galutini_pazymi(s, pasirinkimas);
        return galutinis >= 5.0;
    };
    
    auto partition_point = std::partition(visi_stud.begin(), visi_stud.end(), is_kietakas);
    
    vargsiukai.insert(vargsiukai.end(),
                     std::make_move_iterator(partition_point),
                     std::make_move_iterator(visi_stud.end()));
    
    visi_stud.erase(partition_point, visi_stud.end());
    
    auto end = high_resolution_clock::now();
    
    TestoRezultatai rez;
    rez.skirstymo_laikas = duration_cast<milliseconds>(end - start).count();
    rez.rusiavimo_laikas = 0;
    rez.atmintis_vargsiukai = apskaiciuoti_atminti(vargsiukai);
    rez.atmintis_kietakiai = apskaiciuoti_atminti(visi_stud);
    rez.atmintis_bendra = rez.atmintis_vargsiukai + rez.atmintis_kietakiai;
    return rez;
}

template TestoRezultatai strategija_1<std::vector<Studentas>>(
    const std::vector<Studentas>&, std::vector<Studentas>&, std::vector<Studentas>&, char);
template TestoRezultatai strategija_1<std::list<Studentas>>(
    const std::list<Studentas>&, std::list<Studentas>&, std::list<Studentas>&, char);

template TestoRezultatai strategija_2<std::vector<Studentas>>(
    std::vector<Studentas>&, std::vector<Studentas>&, char);

template TestoRezultatai strategija_3<std::vector<Studentas>>(
    std::vector<Studentas>&, std::vector<Studentas>&, char);

template TestoRezultatai strategija_3_partition<std::vector<Studentas>>(
    std::vector<Studentas>&, std::vector<Studentas>&, char);
