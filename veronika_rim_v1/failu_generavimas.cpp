#include "failu_generavimas.h"
#include <fstream>
#include <random>
#include <string>

int gen_paz(int min, int max){
    static std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

void generuoti_faila(int n, const std::string& failas, int nd_kiekis){
    std::ofstream fd(failas);
    for(int i=1; i<= n; i++){
        fd<<"Vardas "<< i <<" Pavarde "<< i;
        for (int j = 0; j<nd_kiekis; j++){
            fd<< " "<<gen_paz();}
        fd<< " "<<gen_paz();//egzas
        fd<<"\n";
    }
}
