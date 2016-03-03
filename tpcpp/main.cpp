#include <iostream>
#include "multimedia.h"
#include "video.h"
#include "photo.h"
#include "film.h"
#include "group.h"
#include "intrusive_ptr.h"

int main(){
    Video * v1 = new Video("Gattaca",1997,"./films/old/gattaca.mkv",106);
    Photo * p1 = new Photo("Chat qui dort",2002,"./photos/chaton.jpg","Venise");
    Video * v2 = new Video("Inception",2010,"./films/new/inception.avi",116);
    Photo * p2 = new Photo("Central Park",1998,"./photos/central_park.png","New York");
    Film  * f1 = new Film("Le tombeau des lucioles",1988,"./films/old/lucioles.mp4",82);
    long * seq = new long[3];
    seq[0] = 22;    seq[1] = 78;    seq[2] = 89;
    f1->set_table(seq,3);
    delete [] seq; seq = NULL;

    intrusive_ptr<Group> g1 = new Group();
    g1->push_back(p1);
    g1->push_back(v2);
    g1->push_back(v1);
    intrusive_ptr<Group> g2 = new Group();
    g2->push_back(f1);
    g2->push_back(p2);
    g2->push_back(v1);

    g1->print();
    g2->print();

    g1->pop_front();
    g2->pop_front();

    v1->print();

    return 0;
}
