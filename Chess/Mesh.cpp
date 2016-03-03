// --------------------------------------------------------------------------
// Copyright(C) 2009-2015
// Tamy Boubekeur
//                                                                            
// All rights reserved.                                                       
//                                                                            
// This program is free software; you can redistribute it and/or modify       
// it under the terms of the GNU General Public License as published by       
// the Free Software Foundation; either version 2 of the License, or          
// (at your option) any later version.                                        
//                                                                            
// This program is distributed in the hope that it will be useful,            
// but WITHOUT ANY WARRANTY; without even the implied warranty of             
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)           
// for more details.                                                          
// --------------------------------------------------------------------------

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

// recompute minimum and maximum altitude of the mesh
void Mesh::update_alt_offset(){
    float max = -1000000;
    float min = 1000000;
    for (unsigned int i = 0 ; i < V.size() ; i++){
            if ( V[i].p[1] > max )
                max = V[i].p[1];
            if ( V[i].p[1] < min )
                min = V[i].p[1];
    }
    min_alt = min;
    max_alt = max;
}

void Mesh::loadOFF (const std::string & filename) {
	ifstream in (filename.c_str ());
    if (!in) 
        exit (1);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
    update_alt_offset();
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3f (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3f e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3f e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    float maxD = dist (V[0].p, c);
    for (unsigned int i = 0; i < V.size (); i++){
        float m = dist (V[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < V.size (); i++)
        V[i].p = (V[i].p - c) / maxD;
    update_alt_offset();
}

void Mesh::scale(float factor){
    for (unsigned int i = 0 ; i < V.size() ; i++)
        V[i].p *= factor;
    recomputeNormals();
    update_alt_offset();
}

void Mesh::rotate(Axe axe, float angle){
    Vec3f c, tmp;
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    switch (axe){
        case z:
        for (unsigned int i = 0 ; i < V.size() ; i++){
            tmp = V[i].p - c;
            V[i].p[0] = tmp[0] * cos(angle) - tmp[1] * sin(angle);;
            V[i].p[1] = tmp[0] * sin(angle) + tmp[1] * cos(angle);;
        }
        break;
        case y:
        for (unsigned int i = 0 ; i < V.size() ; i++){
            tmp = V[i].p - c;
            V[i].p[0] = tmp[0] * cos(angle) - tmp[2] * sin(angle);;
            V[i].p[2] = tmp[0] * sin(angle) + tmp[2] * cos(angle);;
        }
        break;
        case x:
        for (unsigned int i = 0 ; i < V.size() ; i++){
            tmp = V[i].p - c;
            V[i].p[1] = tmp[1] * cos(angle) - tmp[2] * sin(angle);;
            V[i].p[2] = tmp[1] * sin(angle) + tmp[2] * cos(angle);;
        }
        break;
        default:
        break;
    }
    recomputeNormals();
    update_alt_offset();
}