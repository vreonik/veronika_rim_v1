#pragma once
#include <string>
#include <vector>
#include "studentas.h"

std::vector<Studentas> nuskaityti(const std::string& failas);
std::list<Studentas> nuskaityti_i_list(const std::string& failas);

void isskirti_ir_irasyti(const std::vector<Studentas>& visi_stud,char pasirinkimas, const std::string& failas_vargsiukai, const std::string& failas_kietakiai);
