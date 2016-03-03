#ifndef VIDEO_H
#define VIDEO_H
#include "multimedia.h"
#include <string>

using namespace std;

class Video : public Multimedia
{
protected:
    long duree;

public:
    Video();
    Video(string nom, long date_aquisition, string pathway, long duree);
    virtual ~Video();
    virtual long get_duree();
    virtual void set_duree(long duree);
    virtual void print();
    virtual void play();
};

#endif // VIDEO_H
