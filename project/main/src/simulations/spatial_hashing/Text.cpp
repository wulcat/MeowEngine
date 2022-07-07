//
// Created by Akira Lynn on 02/03/22.
//

#include "Text.h"

void Text::Draw()
{
  unsigned char string[] = "The quick god jumps over the lazy brown fox.";
  int width;
  int i;
  width = glutBitmapLength(GLUT_BITMAP_8_BY_13, string);
  glRasterPos2f(300., 300.);
  float x = .5; /* Centre in the middle of the window */
  glRasterPos2f(x - (float) width / 2, 0.);
  glColor3f(1., 0., 0.);
  int len = 10;
//  for (i = 0; i < len; i++) {
//    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string);
//  }
//  glRasterPos2f(nx+1,y1);
  glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '1');
}

//void static Text::DrawText(const char *string, float x, float y) const
//{
//  glColor3d(1, 1, 1);
//
//  void *font = GLUT_BITMAP_TIMES_ROMAN_24;
//
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glEnable(GL_BLEND);
//
//  int len, i;
//  glRasterPos2d(x, y);
//  len = (int)strlen(string);
//  for (i = 0; i < len; i++)
//  {
//    glutBitmapCharacter(font, string[i]);
//  }
//
//  glDisable(GL_BLEND);
//}

//void RenderString(float x, float y, void *font, const char* string, RGB const& rgb)
//{
//  char *c;
//
//  glColor3f(rgb.r, rgb.g, rgb.b);
//  glRasterPos2f(x, y);
//
//  glutBitmapString(font, string);
//}