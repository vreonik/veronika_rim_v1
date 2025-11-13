#pragma once
#include <string>
#include <vector>
#include <list>
#include "studentas.h"

std::vector<Studentas> nuskaityti(const std::string& failas);
std::list<Studentas> nuskaityti_i_list(const std::string& failas);
