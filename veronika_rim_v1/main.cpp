#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>
#include <fstream>
#include "studentas.h"

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

vector<Studentas> nuskaityti(const string &failas){
    vector<Studentas> visi_stud;
    std::ifstream fd(failas);
    if (!fd.is_open()) {
        cout<<"Nepavyko su failu susitvarkyti"<<endl;
    }
    string eil;
    
    std::getline(fd, eil);
    
    while(std::getline(fd, eil)){
        if(eil.empty()) continue;
        stringstream ss(eil);
        Studentas stud;
        int paz;
        ss>>stud.vard>>stud.pav;
        while(ss>>paz){
            stud.nd.push_back(paz);
        }
        if(!stud.nd.empty()){
            stud.egzas = stud.nd.back();
            stud.nd.pop_back();
        }
        visi_stud.push_back(stud);
    }
    return visi_stud;
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
    
    int limitas = 100000;
    int limito_skaiciuotojas = 0;
    cout<<left<<setw(15)<<"Vardas"<<left<<setw(15)<<"Pavarde";
    if(pasirinkimas == 'v' || pasirinkimas == 'V'){
        cout<<left<<setw(30)<<"Galutinis pažymys(Vid.)";
    }else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
        cout<<left<<setw(30)<<"Galutinis pažymys(Med.)";
    }else{
        cout<<left<<setw(30)<<"Galutinis pažymys(Vid.)"<<left<<setw(30)<<"Galutinis pažymys(Med.)";
    }
    cout<<endl;
    cout<<string(90, '-')<<endl;
    
    cout<<fixed<<setprecision(2);
    for (auto temp : visi_stud) {
        if(limito_skaiciuotojas++ >= limitas)break;
        double vidurkis = skaiciuoti_vidurki(temp.nd);
        double mediana = skaiciuoti_mediana(temp.nd);
        double galutinis_vid = vidurkis*0.4 +temp.egzas*0.6;
        double galutinis_med = mediana*0.4 +temp.egzas*0.6;
        cout<<left<<setw(15)<<temp.vard<<left<<setw(15)<<temp.pav;
        if(pasirinkimas == 'v' || pasirinkimas == 'V'){
            cout<<left<<setw(30)<<galutinis_vid;
        }else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
            cout<<left<<setw(30)<<galutinis_med;
        }else{
            cout<<left<<setw(30)<<galutinis_vid<<left<<setw(30)<<galutinis_med;
        }
        cout<<endl;
    }
    if(visi_stud.size() > limitas)
        cout << "... ir dar " << (visi_stud.size() - limitas) << " įrašų"<<endl;
        return 0;
}
