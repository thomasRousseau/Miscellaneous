#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <GL/glut.h>
#include <math.h>
#include <typeinfo>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Graphics.h"
#include "ChessPieces.h"
#include "Game.h"

using namespace std;

void update_window_title(string s);
void display();
void reset_counter();

#endif