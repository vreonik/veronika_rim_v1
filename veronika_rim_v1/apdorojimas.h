#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "studentas.h"

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

extern bool testavimo_rezimas;

template<typename Container>
void spausdinti_perziura(const Container& visi_stud, char pasirinkimas, int limitas = 10);

template<typename Container>
void klasifikuoti_ir_irasyti(const Container &visi_stud,
                             char pasirinkimas,
                             const std::string &failas_vargsiukai,
                             const std::string &failas_kietakiai,
                             long long &skirstymo_ms,
                             long long &rusiavimo_ms,
                             long long &irasymo_ms,
                             char rikiuoti_kriterijus = 'v');

template<typename Container>
void apdoroti_faila(const std::string &fname, char budas, char rikiavimas);
