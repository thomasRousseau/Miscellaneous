#include "table.h"

Table::Table(){}

Table::~Table(){}

const intrusive_ptr<Film> Table::addFilm(int duration, string name, unsigned int date, string pathname){
    intrusive_ptr<Film> f = new Film(name,date,pathname,duration);
    this->filesMap[name] = f;
    return f;
}

const intrusive_ptr<Photo> Table::addPhoto(string place, string name, unsigned int date, string pathname){
    intrusive_ptr<Photo> p = new Photo(name,date,pathname,place);
    this->filesMap[name] = p;
    return p;
}

const intrusive_ptr<Video> Table::addVideo(int duration, string name, unsigned int date, string pathname){
    intrusive_ptr<Video> v = new Video(name,date,pathname,duration);
    this->filesMap[name] = v;
    return v;
}

const intrusive_ptr<Group> Table::addGroup(string name){
    intrusive_ptr<Group> g = new Group(name);
    this->groupsMap[name] = g;
    return g;
}

void Table::deleteFile(string name){
    map<string, intrusive_ptr<Group> >::iterator i;
    Group::iterator j;
    for(i=this->groupsMap.begin();i!=this->groupsMap.end();++i){
        intrusive_ptr<Group> g = i->second;
        for(j=g->begin();j!=g->end();++j){
            if((*j)->get_nom()==name)
                j=g->erase(j);
        }
    }
    this->filesMap.erase(name);
}

void Table::deleteGroup(string name){
    this->groupsMap.erase(name);
}

bool Table::searchFile(string name) const{
    map<string, intrusive_ptr<Multimedia> >::const_iterator i;
    i = this->filesMap.find(name);
    if(i!=this->filesMap.end()){
        cout << "Fichier "<< name <<" trouvé : " << endl;
        i->second->print();
        return true;
    }
    else{
        cout << "Fichier "<< name <<" non trouvé" << endl;
        return false;
    }
}

bool Table::searchGroup(string name) const{
    map<string, intrusive_ptr<Group> >::const_iterator i;
    i = this->groupsMap.find(name);
    if(i!=this->groupsMap.end()){
        cout << "Groupe "<<name<<" trouvé, contenu :" << endl;
        intrusive_ptr<Group> g = i->second;
        Group::iterator j;
        for(j=g->begin();j!=g->end();++j)
            (*j)->print();
        return true;
    }
    else{
        cout << "Groupe "<<name<<" non trouvé"<< endl;
        return false;
    }
}

void Table::play(string name) const{
    map<string, intrusive_ptr<Multimedia> >::const_iterator i;
    i = filesMap.find(name);
    if(i != filesMap.end())
        i->second->play();
}
