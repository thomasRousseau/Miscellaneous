#include <iostream>
#include "photo.h"

using namespace std;

Photo::Photo() :
    Multimedia(),place("undefined"){}

Photo::Photo(string _n, long _d, string _p, string _pl):
    Multimedia(_n,_d,_p),place(_pl){}

Photo::~Photo(){}

void Photo::set_place(string _pl){this->place = _pl;}

string Photo::get_place(){return this->place;}

void Photo::print(){
    cout << "Photo : " << this->nom << ", " << this->date_aquisition << ", in " << this->place <<  " (" << this->pathway << ")"<< endl;
}

void Photo::play(){
    system(("xdg-open "+this->pathway+" &").c_str());}
