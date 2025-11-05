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
#include "util.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::fixed;
using std::setprecision;
using std::sort;
using std::setw;
using std::left;
using std::right;
using std::pair;
using std::ofstream;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

extern bool testavimo_rezimas;

template<typename Container>
void spausdinti_perziura(const Container& visi_stud, char pasirinkimas, int limitas = 10);

template<typename Container>
void klasifikuoti_ir_irasyti(const Container &visi_stud,
                             char pasirinkimas,
                             const string &failas_vargsiukai,
                             const string &failas_kietakiai,
                             long long &skirstymo_ms,
                             long long &rusiavimo_ms,
                             long long &irasymo_ms,
                             char rikiuoti_kriterijus = 'v');

template<typename Container>
void apdoroti_faila(const string &fname, char budas, char rikiavimas);
