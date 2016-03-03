#ifndef GROUP_H
#define GROUP_H
#include <list>
#include <string>
#include "multimedia.h"
#include "intrusive_ptr.h"

using namespace std;

class Group : public list<intrusive_ptr<Multimedia> >, public Pointable
{
private:
    string name;

public:
    Group();
    Group(string n);
    virtual string getName();
    ~Group();
    virtual void print();
};

#endif // GROUP_H
