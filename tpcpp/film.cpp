#include <iostream>
#include "film.h"
#include "video.h"

Film::Film() : Video(){}

Film::Film(string _n, long _d, string _p, long _du):
    Video(_n, _d, _p, _du){}

Film::~Film(){}

void Film::set_table(long *t, int _nb){
    this->nb_seq = _nb;
    this->table = new long[_nb];
    for(int i = 0 ; i < _nb ; i++)
        table[i] = t[i];}

long * Film::get_table(){return this->table;}

int Film::get_nb_seq(){return this->nb_seq;}

void Film::print(){
    cout << "Film : " << this->get_nom() << ", "
         << this->date_aquisition << ", durée " << this->get_duree()
         << " minutes, " << this->nb_seq << " chapitres [";
    for(int i = 0 ; i < this->get_nb_seq() ; i++){
        cout << this->get_table()[i];
        if(i!=this->get_nb_seq()-1)
            cout <<", ";}
    cout << "] (" << this->pathway << ")"<< endl;
}

void Film::play(){
    system(("mplayer "+this->pathway+" &").c_str());}
