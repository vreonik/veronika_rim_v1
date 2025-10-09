#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>
#include <fstream>
#include <chrono>


#include "studentas.h"
#include "nuskaityti.h"
#include "failu_generavimas.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::fixed;
using std::setprecision;
using std::sort;
using std::stringstream;
using std::max;
using std::min;
using std::setw;
using std::left;
using std::right;
using std::pair;
using std::ofstream;

using Laikmatis = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

static string be_priesdelio(const string &kelias){
    auto pos = kelias.find_last_of("/\\");
    string vardas = (pos == string::npos) ? kelias : kelias.substr(pos + 1);
    auto taskas = vardas.find_last_of('.');
    if(taskas == string::npos) return vardas;
    return vardas.substr(0, taskas);
}

void spausdinti_perziura(const vector<Studentas>& visi_stud, char pasirinkimas, int limitas = 10) {
    cout<<left<<setw(15)<<"Vardas"<<left<<setw(15)<<"Pavarde";
    if(pasirinkimas == 'v' || pasirinkimas == 'V'){
        cout<<left<<setw(30)<<"Galutinis(Vid.)";
    }else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
        cout<<left<<setw(30)<<"Galutinis(Med.)";
    }else{
        cout<<left<<setw(30)<<"Galutinis(Vid.)"<<left<<setw(30)<<"Galutinis(Med.)";
    }
    cout<<endl<<string(90, '-')<<endl;
    cout<<fixed<<setprecision(2);

    int kiek = 0;
    for(const auto &s : visi_stud){
        if(kiek++ >= limitas) break;
        double vid = skaiciuoti_vidurki(s.nd);
        double med = skaiciuoti_mediana(s.nd);
        double gvid = vid * 0.4 + s.egzas * 0.6;
        double gmed = med * 0.4 + s.egzas * 0.6;
        cout<<left<<setw(15)<<s.vard<<left<<setw(15)<<s.pav;
        if (pasirinkimas == 'v' || pasirinkimas == 'V'){
            cout<<left<<setw(30)<<gvid;
        } else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
            cout<<left<<setw(30)<<gmed;
        } else{
            cout<<left<<setw(30)<<gvid<<left<<setw(30)<<gmed;
        }
        cout<<endl;
    }
    if((int)visi_stud.size() > limitas)
        cout<< "... ir dar "<<(visi_stud.size() - limitas)<< " irasai"<<endl;
}

void klasifikuoti_ir_irasyti(const vector<Studentas>& visi_stud, char pasirinkimas, const string &failas_vargsiukai, const string &failas_kietakiai, long long &klasifik_ms, long long &irasymo_ms){
    auto pradzia = Laikmatis::now();

    vector<pair<Studentas, double>> vargsiukai, kietakiai;
    for (const auto &s : visi_stud){
        double galutinis = 0;
        if (pasirinkimas == 'v' || pasirinkimas == 'V')
            galutinis = skaiciuoti_vidurki(s.nd) * 0.4 + s.egzas * 0.6;
        else
            galutinis = skaiciuoti_mediana(s.nd) * 0.4 + s.egzas * 0.6;

        if (galutinis < 5.0) vargsiukai.emplace_back(s, galutinis);
        else kietakiai.emplace_back(s, galutinis);
    }

    sort(vargsiukai.begin(), vargsiukai.end(),
         [](auto &a, auto &b) { return a.first.vard < b.first.vard; });
    sort(kietakiai.begin(), kietakiai.end(),
         [](auto &a, auto &b) { return a.first.vard < b.first.vard; });

    auto vidurys = Laikmatis::now();
    klasifik_ms = std::chrono::duration_cast<ms>(vidurys - pradzia).count();

    auto ras_pradzia = Laikmatis::now();
    ofstream fv(failas_vargsiukai);
    ofstream fk(failas_kietakiai);
    fv << fixed << setprecision(2);
    fk << fixed << setprecision(2);
    for (auto &p : vargsiukai) fv << p.first.vard << " " << p.first.pav << " " << p.second << "\n";
    for (auto &p : kietakiai) fk << p.first.vard << " " << p.first.pav << " " << p.second << "\n";
    auto ras_pabaiga = Laikmatis::now();

    irasymo_ms = std::chrono::duration_cast<ms>(ras_pabaiga - ras_pradzia).count();
}


int main(){
    vector<Studentas> visi_stud;
    char pasirink;
    cout<<"Ar norite skaityti iš failo(f) ar įvesti/generuoti randomu patys(p)? ";
    cin>>pasirink;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (pasirink == 'f' || pasirink == 'F'){
        visi_stud = nuskaityti("kursiokai.txt");
        sort(visi_stud.begin(), visi_stud.end(), [](const Studentas &a, const Studentas &b){
            return a.vard < b.vard;
        });
    }else{
        char dar = 't';
        
        while(dar == 't' || dar == 'T'){
            Studentas stud;
            cout<<"Įveskite studento vardą: ";
            cin>>stud.vard;
            cout<<"Įveskite studento pavardę: ";
            cin>>stud.pav;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            char pas;
            cout<<"Generuosite random (r) ar įvesite (i)? ";
            cin>>pas;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if(pas == 'i' || pas == 'I'){
                cout<<"Įveskite N.d. pažymius po vieną. Pabaigus įvedimą paspauskite du kartus ENTER:"<<endl;
                while(true){
                    string eil;
                    cout<<"Pažymys: ";
                    getline(cin, eil);
                    if(eil.empty())break;
                    
                    stringstream ss(eil);
                    int nd;
                    if(ss >> nd){
                        stud.nd.push_back(nd);
                    } else{
                        cout<<"Blogai, įveskite iš naujo!:("<<endl;
                    }
                }
                cout<<"Įveskite egzamino pažymį: ";
                cin>>stud.egzas;
                cin.ignore();}
            else if(pas == 'r' || pas == 'R'){
                int kiek_nd;
                cout<<"Kiek namų darbų generuoti?";
                cin >> kiek_nd;
                cin.ignore();
                
                for(int i=0; i<kiek_nd; i++){
                    int nd = gen_paz();
                    stud.nd.push_back(nd);
                }
                stud.egzas = gen_paz();
                cout<<"Sugeneruoti paž.: ";
                for( auto x: stud.nd) cout<<x<<" ";
                cout<<"Gen. egz. paž.: "<<stud.egzas<<endl;
            } else{
                cout<<"Kažkas blogai, grįžtame į meniu."<<endl;
                continue;}
            visi_stud.push_back(stud);
            
            cout<<"Pridėti dar vieną?(t/T - taip, kitaip - ne): ";
            cin>>dar;}
    }
    sort(visi_stud.begin(), visi_stud.end(), [](const Studentas &a, const Studentas &b){
        return a.vard < b.vard;
    });
    char pasirinkimas;
    cout<<"Pasirinkite galutinio balo skaičiavimo būdą(v-vidurkis, m-mediana, a-abudu): ";
    cin>>pasirinkimas;
    
    return 0;
}
