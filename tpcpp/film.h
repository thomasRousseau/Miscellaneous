#ifndef FILM_H
#define FILM_H
#include "video.h"

class Film : public Video
{
protected:
    long *table;
    int nb_seq;

public:
    Film();
    Film(string nom, long date_aquisition, string pathway, long duree);
    virtual ~Film();
    virtual void set_table(long *t,int nb);
    virtual long* get_table();
    virtual int get_nb_seq();
    virtual void print();
    virtual void play();
};

#endif // FILM_H
