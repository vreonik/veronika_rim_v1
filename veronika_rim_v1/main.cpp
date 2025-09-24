#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::setw;
using std::left;
using std::right;
using std::fixed;
using std::setprecision;
using std::sort;

struct Studentas{
    string vard;
    string pav;
    vector<int> nd;
    int egzas;
};

int main(){
    int n, m;
    double sum=0, med=0;
    char pasirinkimas;
    Studentas stud;
    cout<<"Įveskite studento vardą: ";
    cin>>stud.vard;
    cout<<"Įveskite studento pavardę: ";
    cin>>stud.pav;
    
    cout<<"Kiek pažymių turi studentas?: ";
    cin>>n;
    
    for(int i=0; i<n; i++){
        cout<<"Įveskite "<<i+1<<" Pažymį iš "<<n<<": ";
        cin>>m;
        stud.nd.push_back(m);
        sum+=m;
    }
    
    vector<int> nd_sortintas = stud.nd;
    sort(nd_sortintas.begin(), nd_sortintas.end());
    
    if(n%2 == 0){
        med = (nd_sortintas[n/2 - 1] + nd_sortintas[n/2]) / 2.0;
    }
    else{
        med = nd_sortintas[n/2];
    }
    cout<<"Įveskite egzamino pažymį: ";
    cin>>stud.egzas;
    
    double vidurkis = sum/n;
    double galutinis_vid = vidurkis*0.4 + stud.egzas*0.6;
    double galutinis_med = med*0.4 + stud.egzas*0.6;
    
    cout << "Pasirinkite galutinio balo skaičiaivimo būdą (įrašykite pirmą raidę) - vidurkis, mediana ar abudu: ";
    cin >> pasirinkimas;
    cout<<fixed<<setprecision(2);
    if(pasirinkimas == 'v' || pasirinkimas == 'V'){
        cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažimys(Vid.) "<< galutinis_vid<<endl;
    } else if(pasirinkimas == 'm' || pasirinkimas == 'M'){
        cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažimys(Med.) "<< galutinis_med<<endl;
    } else{
        cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažimys(Vid.) "<< galutinis_vid<<endl;
        cout<<stud.vard<<" "<<stud.pav<<"; Galutinis pažimys(Med.) "<< galutinis_med<<endl;
    }
    
    
}
