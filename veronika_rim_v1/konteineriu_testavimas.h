#pragma once
#include <string>
#include <vector>
#include <list>
#include "studentas.h"
#include "strategijos.h" // PRIDĖTI ŠITĄ!

void testuoti_konteinerius_su_failu(const std::string& fname);
void testuoti_konteinerius_sugeneruotus();
void testuoti_konteinerius_su_pasirinktu_failu();

void testuoti_visas_strategijas(const std::string& fname);
void testuoti_strategijas_su_visais_dydziais();

void atlikti_isamiai_analize(const std::string& fname, int testu_kartai = 3);
void palyginti_strategijas_pagal_dydi();
