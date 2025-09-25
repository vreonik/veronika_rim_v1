#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>

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

int main(){
    vector<Studentas> visi_stud;
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
            cin.ignore();}else if(pas == 'r' || pas == 'R'){
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
                   continue;
                   }
        
        if (stud.nd.empty()){
            cout<<"Studentas neturi namų darbų pažymių!"<<endl;
            continue;
        }

        double vidurkis = sum / stud.nd.size();

        vector<int> nd_sortintas = stud.nd;
        sort(nd_sortintas.begin(), nd_sortintas.end());
        if(stud.nd.size() % 2 == 0){
            med = (nd_sortintas[stud.nd.size() / 2 - 1] + nd_sortintas[stud.nd.size() / 2]) / 2.0;
        } else{
            med = nd_sortintas[stud.nd.size() / 2];
        }

        double galutinis_vid = vidurkis * 0.4 + stud.egzas * 0.6;
        double galutinis_med = med * 0.4 + stud.egzas * 0.6;

        char pasirinkimas;
        cout<<"Pasirinkite galutinio balo skaičiavimo būdą(v-vidurkis, m-mediana, a-abudu): ";
        cin>>pasirinkimas;
        cin.ignore();

        cout<<fixed<<setprecision(2);
        if(pasirinkimas == 'v' || pasirinkimas == 'V'){
            cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažymys(Vid.) "<< galutinis_vid<<endl;
        }else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
            cout<<stud.vard<<" " <<stud.pav<<"; Galutinis pažymys(Med.) "<<galutinis_med<< endl;
        }else{
            cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažymys(Vid.) "<< galutinis_vid<<endl;
            cout<<stud.vard<<" " <<stud.pav<<"; Galutinis pažymys(Med.) "<<galutinis_med<< endl;
        }

        visi_stud.push_back(stud);

        cout<<"Pridėti dar vieną?(t/T - taip, kitaip - ne): ";
        cin>>dar;
        cin.ignore();
    }

    cout<<"Visų studentų sąrašas:"<<endl;
    for (auto temp : visi_stud) {
        cout<<temp.vard<<" "<<temp.pav<<"; N.D. skaičius: "<<temp.nd.size()<<endl;
    }

    return 0;
}
