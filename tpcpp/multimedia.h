#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H
#include <string>
#include <cstring>
#include <cstdlib>

#define SMART_PTR_DEBUG
#define SMART_PTR_DEBUG_MESSAGES

#include "intrusive_ptr.h"

using namespace std;

class Multimedia : public Pointable
{
    protected:
        string nom;
        long date_aquisition;
        string pathway;
    public:
        Multimedia();
        Multimedia(string nom, long date_aquisition, string pathway);
        virtual ~Multimedia();
        virtual string get_nom();
        virtual long get_date_aquisition();
        virtual string get_pathway();
        virtual void set_nom(string);
        virtual void set_date_aquisition(long);
        virtual void set_pathway(string);
        virtual void print() const;
        virtual void play();
};

#endif // MULTIMEDIA_H
