#ifndef PHOTO_H
#define PHOTO_H
#include "multimedia.h"
#include <string>

using namespace std;

class Photo : public Multimedia
{
private:
    string place;

public:
    Photo();
    Photo(string nom, long date_aquisition, string pathway, string place);
    virtual ~Photo();
    virtual string get_place();
    virtual void set_place(string place);
    virtual void print();
    virtual void play();
};

#endif // PHOTO_H
