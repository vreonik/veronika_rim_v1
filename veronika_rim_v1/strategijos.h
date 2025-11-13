#pragma once
#include "studentas.h"
#include <vector>
#include <list>

enum class Strategija {
    STRATEGIJA_1,  // Du nauji konteineriai
    STRATEGIJA_2,  // Vienas naujas + trynimas
    STRATEGIJA_3   // STL algoritmai
};

struct TestoRezultatai {
    long long skirstymo_laikas;
    long long rusiavimo_laikas;
    long long irasymo_laikas;
    size_t atmintis_vargsiukai;
    size_t atmintis_kietakiai;
};

// 1 strategija - du nauji konteineriai
template<typename Container>
TestoRezultatai strategija_1(const Container& visi_stud,
                            Container& vargsiukai,
                            Container& kietakiai,
                            char pasirinkimas);

// 2 strategija - vienas naujas konteineris + trynimas
template<typename Container>
TestoRezultatai strategija_2(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas);

// 3 strategija - STL algoritmai
template<typename Container>
TestoRezultatai strategija_3(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas);
