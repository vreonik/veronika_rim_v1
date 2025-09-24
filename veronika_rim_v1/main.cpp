#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

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

struct Studentas{
    string vard;
    string pav;
    vector<int> nd;
    int egzas;
};

int main(){
    int n, m;
    double sum=0;
    Studentas stud;
    cout<<"Įveskite studento vardą: ";
    cin>>stud.vard;
    cout<<"Įveskite sutdento pavardę: ";
    cin>>stud.pav;
    
    cout<<"Kiek pažymių turi studentas?: ";
    cin>>n;
    
    for(int i=0; i<n; i++){
        cout<<"Įveskite "<<i+1<<" Pažymį iš "<<n<<": ";
        cin>>m;
        stud.nd.push_back(m);
        sum+=m;
        
    }
    cout<<"Įveskite egzamino pažymį: ";
    cin>>stud.egzas;
    double vidurkis = sum/n;
    double galutinis = vidurkis*0.4 + stud.egzas*0.6;
    cout<<fixed<<setprecision(2)<<stud.vard<<" "<<stud.pav<<"; Galutinis pažimys(Vid.) "<< galutinis<<endl;
    
}
