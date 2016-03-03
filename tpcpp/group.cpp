#include "group.h"

Group::Group():name("Untitled"){}

Group::Group(string n):name(n){}

string Group::getName(){return this->name;}

void Group::print(){
    for(list<intrusive_ptr<Multimedia> >::iterator i=begin();i!=end();++i)
        (*i)->print();
}

Group::~Group(){

}
