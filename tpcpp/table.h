#include <map>
#include "group.h"
#include "photo.h"
#include "film.h"

#ifndef TABLE_H
#define TABLE_H

class Table
{
    map<string, intrusive_ptr<Multimedia> > filesMap;
    map<string, intrusive_ptr<Group> > groupsMap;

public:
    Table();
    virtual ~Table();

    const intrusive_ptr<Photo> addPhoto(string _place, string name, unsigned int date, string pathname);
    const intrusive_ptr<Video> addVideo(int _duration, string name, unsigned int date, string pathname);
    const intrusive_ptr<Film> addFilm(int _duration, string name, unsigned int date, string pathname);

    const intrusive_ptr<Group> addGroup(string name);

    void deleteFile(string name);
    void deleteGroup(string name);

    bool searchFile(string name) const;
    bool searchGroup(string name) const;

    void play(string name) const;
};
#endif // TABLE_H
