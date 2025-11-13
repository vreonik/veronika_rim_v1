#pragma once
#include "studentas.h"
#include <vector>
#include <list>

struct TestoRezultatai {
    long long skirstymo_laikas;
    size_t atmintis_vargsiukai;
    size_t atmintis_kietakiai;
    size_t atmintis_bendra;
};

template<typename Container>
TestoRezultatai strategija_1(const Container& visi_stud,
                            Container& vargsiukai,
                            Container& kietakiai,
                            char pasirinkimas);

TestoRezultatai strategija_2(std::vector<Studentas>& visi_stud,
                            std::vector<Studentas>& vargsiukai,
                            char pasirinkimas);

TestoRezultatai strategija_2(std::list<Studentas>& visi_stud,
                            std::list<Studentas>& vargsiukai,
                            char pasirinkimas);

TestoRezultatai strategija_3(std::vector<Studentas>& visi_stud,
                            std::vector<Studentas>& vargsiukai,
                            char pasirinkimas);

TestoRezultatai strategija_3(std::list<Studentas>& visi_stud,
                            std::list<Studentas>& vargsiukai,
                            char pasirinkimas);
