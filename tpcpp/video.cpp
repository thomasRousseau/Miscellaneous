#include <iostream>
#include "video.h"

using namespace std;

Video::Video() :
    Multimedia(),duree(0){}

Video::Video(string _n, long _d, string _p, long _du) :
    Multimedia(_n,_d,_p),duree(_du){}

Video::~Video(){}

void Video::set_duree(long _du){this->duree = _du;}

long Video::get_duree(){return this->duree;}

void Video::print(){
    cout << "Video : " << this->nom << ", "
         << this->date_aquisition << ", durée "
         << this->duree <<  " minutes (" << this->pathway << ")"<< endl;
}

void Video::play(){
    system(("mplayer "+this->pathway+" &").c_str());}
