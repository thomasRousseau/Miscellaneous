#include <iostream>
#include "multimedia.h"

using namespace std;

Multimedia::Multimedia():
    date_aquisition(0),nom("untitled"),pathway("./"){}

Multimedia::Multimedia(string _n, long _d, string _p):
    nom(_n),date_aquisition(_d),pathway(_p){}

Multimedia::~Multimedia(){cout << "Destruction de "<<this->get_nom()<<endl;}

string Multimedia::get_nom(){return this->nom;}
long Multimedia::get_date_aquisition(){return this->get_date_aquisition();}
string Multimedia::get_pathway(){return this->get_pathway();}

void Multimedia::set_nom(string _n){this->nom = _n;}
void Multimedia::set_date_aquisition(long _d){this->date_aquisition = _d;}
void Multimedia::set_pathway(string _p){this->pathway = _p;}

void Multimedia::print() const{
    cout << "Multimedia : " << this->nom << ", " << this->date_aquisition << " (" << this->pathway << ")"<< endl;}

void Multimedia::play(){}
