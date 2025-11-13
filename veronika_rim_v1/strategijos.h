#pragma once
#include "studentas.h"
#include <vector>
#include <list>

enum class Strategija {
    STRATEGIJA_1,
    STRATEGIJA_2,
    STRATEGIJA_3
};

struct TestoRezultatai {
    long long skirstymo_laikas;
    long long rusiavimo_laikas;
    long long irasymo_laikas;
    size_t atmintis_vargsiukai;
    size_t atmintis_kietakiai;
    size_t atmintis_bendra;
};

template<typename Container>
TestoRezultatai strategija_1(const Container& visi_stud,
                            Container& vargsiukai,
                            Container& kietakiai,
                            char pasirinkimas);

template<typename Container>
TestoRezultatai strategija_2(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas);

template<typename Container>
TestoRezultatai strategija_3(Container& visi_stud,
                            Container& vargsiukai,
                            char pasirinkimas);

template<typename Container>
TestoRezultatai strategija_3_partition(Container& visi_stud,
                                      Container& vargsiukai,
                                      char pasirinkimas);
