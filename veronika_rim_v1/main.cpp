#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>
#include <fstream>

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

struct Studentas{
    string vard;
    string pav;
    vector<int> nd;
    int egzas;
};

int gen_paz(int min=1, int max=10){
    static std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

vector<Studentas> nuskaityti(const string &failas){
    vector<Studentas> visi_stud;
    std::ifstream fd(failas);
    string eil;
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
    }else{
        char dar = 't';
        
        while(dar == 't' || dar == 'T'){
            Studentas stud;
            double sum = 0, med = 0;
            
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
                        sum += nd;
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
                        sum+=nd;
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
            cin>>dar;
            cin.ignore();
    }

        char pasirinkimas;
        cout<<"Pasirinkite galutinio balo skaičiavimo būdą(v-vidurkis, m-mediana, a-abudu): ";
        cin>>pasirinkimas;

        cout<<left<<setw(15)<<"Vardas"<<left<<setw(15)<<"Pavarde";
        if(pasirinkimas == 'v' || pasirinkimas == 'V'){
            cout<<left<<setw(20)<<"Galutinis pažymys(Vid.)";
        }else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
            cout<<left<<setw(20)<<"Galutinis pažymys(Med.)";
        }else{
            cout<<left<<setw(20)<<"Galutinis pažymys(Vid.)"<<left<<setw(20)<<"Galutinis pažymys(Med.)";
        }
        cout<<endl;
        cout<<string(60, '-')<<endl;
    }
    
    cout<<fixed<<setprecision(2);
    for (auto temp : visi_stud) {
        double vidurkis = skaciuoti_vidurki(stud.nd);//susikurt funckija
        double mediana = skaiciuoti_mediana(stud.nd);//susikurti funkcija
        double galutinis_vid = vidurkis*0.4 +stud.egzas*0.6;
        double galutinis_med = mediana*0.4 +stud.egzas*0.6;
        cout<<temp.vard<<" "<<temp.pav<<"; N.D. skaičius: "<<temp.nd.size()<<endl;
    }

    return 0;
}
