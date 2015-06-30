#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glut.h>

extern void *font;
extern int B[4];

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};


extern GLuint theTexture[80];
extern char* textureLists[49];

int ImageLoad(const char *filename, Image *image);
void LoadGLTextures(int k);
void glprint(float x, float y, char *string);//屏幕上显示文字的函数
#endif // TEXTURE_H_INCLUDED
