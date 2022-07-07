//
// Created by Akira Lynn on 02/03/22.
//

#ifndef ALGORITHMS_TEXT_H
#define ALGORITHMS_TEXT_H

#include <GLUT/glut.h>
#include "GLUT/glutbitmap.h"
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "string"
//#include < GL/freeglut.h>
//#include "OpenGL/Free"

using namespace std;

class Text
{
public:
//  Text();
//  ~Text();
  void Draw();
  static void DrawText(const char* string, float x, float y)
  {
    glColor3d(1, 1, 1);

    void *font = GLUT_BITMAP_TIMES_ROMAN_24;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    int len, i;
    glRasterPos2d(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
      glutBitmapCharacter(font, string[i]);
    }

    glDisable(GL_BLEND);
  }

};

#endif // ALGORITHMS_TEXT_H
