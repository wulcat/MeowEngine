#include "GLUT/glut.h"
#include "Game.h"
#include "Text.h"
#include "Maths.h"
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "math.h"
#include <iostream>

using namespace std;

class Game;
class Text;

Game* game = new Game();
Text* text = new Text();

void Resize(GLint w, GLint h)
{
  glViewport(0, 0, w, h);
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h) {
    // width is smaller, go from -50 .. 50 in width
    glOrtho(-50.0, 50.0, -50.0/aspect, 50.0/aspect, -1.0, 1.0);
  } else {
    // height is smaller, go from -50 .. 50 in height
    glOrtho(-50.0*aspect, 50.0*aspect, -50.0, 50.0, -1.0, 1.0);
  }
}

void Render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  game->Render();
//  text->Draw();
//  Text::DrawText("dsds", 100 , 100);
//  text->DrawText("dsds", 100 , 100);
  glFlush();
  glutSwapBuffers();
}

void Update(int v)
{
  game->Update();

  glutPostRedisplay();

  glutTimerFunc(1000/60, Update, v);
}

void MouseClick(int button, int state, int x, int y)
{
  game->MouseClick(button, state, x, y);
}

void MouseMove(int x, int y)
{
  game->MouseUpdate(x, y);
}

void DrawText()
{

}
//void Mouse

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  // giving window size in X- and Y- direction
  glutInitWindowSize(game->mapWidth, game->mapHeight);
  glutInitWindowPosition(0, 0);

  // Giving name to window
  glutCreateWindow("Spatial Hashing");

  glClearColor(0.0,0.0,0.0,0.0);

  glColor3f(0.0, 1.0, 0.0);

  glPointSize(1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // setting window dimension in X- and Y- direction
  gluOrtho2D(0, game->mapWidth, 0, game->mapHeight);
//  glutReshapeFunc(Resize);
  glutDisplayFunc(Render);
  glutMouseFunc(MouseClick);
  glutMotionFunc(MouseMove);
  glutTimerFunc(100, Update, 0);
  glutMainLoop();

  return 0;
}


