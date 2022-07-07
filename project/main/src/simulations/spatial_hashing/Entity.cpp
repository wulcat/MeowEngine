//
// Created by Akira Lynn on 28/02/22.
//
#include "OpenGL/gl.h"
#include "math.h"
#include "iostream"

#include "Entity.h"
#include "Vector3.h"
#include "Maths.h"
#include "Text.h"

Entity::Entity(float x, float y, float z, float scale)
{
  position = new Vector3();
  SetPosition(x,y,z);
  SetSize(scale);
}

Entity::~Entity() {}

void Entity::Render()
{
  glBegin(GL_POINTS);
  if(isActive)
  {
    glColor3f(1,0,0);
  }
  else {
    glColor3f(0,1,0);
  }

  float i, x ,y;
  for ( i = 0; i < (2 * Maths::PI); i += 0.001)
    {
      // let 200 is radius of circle and as,
      // circle is defined as x=r*cos(i) and y=r*sin(i)
      x = size * cos(i) + position->x;
      y = size * sin(i) + position->y;

      glVertex2i(x, y);
    }

    glEnd();
//    const string te = to_string(cellID);
    Text::DrawText(to_string(cellID).c_str(),position->x,position->y);
}



void Entity::Update()
{
//  std::cout<<position->y<<'\n';
}

Vector3 Entity::GetPosition()
{
  return *position;
}

float Entity::GetSize()
{
  return size;
}

void Entity::SetPosition(float x, float y, float z)
{
  position->x = x;
  position->y = y;
  position->z = z;
}

void Entity::SetSize(float scale)
{
  this->size = scale;
}
