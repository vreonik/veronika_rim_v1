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
