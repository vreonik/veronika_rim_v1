#include <fstream>
#include <random>
#include "failu_generavimas.h"

int gen_paz(int min, int max){
    static std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

void generuoti_faila(int n, const std::string& failas, int nd_kiek){
    std::ofstream fs(failas);
    fs << "Vardas Pavarde";
    for (int i = 1; i <= nd_kiek; i++)
        fs << " ND" << i;
    fs << " Egzaminas\n";

    for (int i = 1; i <= n; i++) {
        fs << "Vardas" << i << " Pavarde" << i << " ";
        for (int j = 0; j < nd_kiek; j++)
            fs << gen_paz(1, 10) << " ";
        fs << gen_paz(1, 10) << "\n";
    }
}

