#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <algorithm>

#define MEDIUM_TEXTURE "texture/medium-wood.data"

typedef unsigned char BYTE;

extern GLuint medium_texture;

GLuint loadTextureRAW(const char * imagepath, bool wrap);
void init_textures();

#endif