#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <list>
#include "studentas.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::list;
using std::ofstream;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

void testuoti_konteinerius_su_failu(const string& fname);
void testuoti_konteinerius_sugeneruotus();
void testuoti_konteinerius_su_pasirinktu_failu();
void testuoti_konteinerius_su_vidurkiais();

void testuoti_visas_strategijas(const string& fname);
void testuoti_strategijas_su_visais_dydziais();

void atlikti_isamiai_analize(const string& fname, int testu_kartai = 5);
void palyginti_strategijas_pagal_dydi();
void atlikti_greicio_analize();

template<typename Container>
void testuoti_strategija_su_vidurkiu(const string& fname,
                                   char b,
                                   char rikiuoti_kriterijus,
                                   int testu_kartai,
                                   ofstream& rezultatai,
                                   int strategijos_nr);

extern template void testuoti_strategija_su_vidurkiu<vector<Studentas>>(
    const string&, char, char, int, ofstream&, int);
extern template void testuoti_strategija_su_vidurkiu<list<Studentas>>(
    const string&, char, char, int, ofstream&, int);
