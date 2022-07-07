//
// Created by Akira Lynn on 28/02/22.
//

#include "Game.h"
#include "Entity.h"
#include "GLUT/glut.h"
#include "Maths.h"
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "SpatialHashing.h"
#include "math.h"
#include "random"

Game::Game()
{
  spatialHashing = new SpatialHashing(mapWidth,mapHeight,cellSize);
  player = new Entity(200,200,0,20);

  spatialHashing->InsertEntity(player);

  for(int i = 0 ; i < 50 ; i++)
  {
    std::random_device r;
    std::default_random_engine engine(r());

    std::uniform_int_distribution<int> uniformIntDistributionWidth(0, mapWidth);
    std::uniform_int_distribution<int> uniformIntDistributionHeight(0, mapHeight);

    float x,y;
    x = uniformIntDistributionWidth(r);
    y = uniformIntDistributionHeight(r);

//    std::cout<<x<<"\n";
    Entity* entity = new Entity(x,y,0, 20);
    entities.push_back(entity);

    spatialHashing->InsertEntity(entity);
  }
}

Game::~Game() {}

void Game::Render()
{

//  glBegin(GL_POINTS);

  float x1,y1,x2,y2;

  // Draw Grid
  glColor3f(0,0,1);
  glBegin(GL_POINTS);
  for(int i = 0 ; i < spatialHashing->columns; i++)
  {
    for(int j = 0 ; j < spatialHashing->rows; j++)
    {
      x1 = i * cellSize;
      y1 = j * cellSize;
      x2 = (i + 1) * cellSize;
      y2 = (j + 1) * cellSize;

      glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x1, y2);
      glEnd();

      glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
      glEnd();
    }
  }
  glEnd();

  player->Render();

  for(EntityList::iterator iterator = entities.begin(), end = entities.end();
       iterator != end;
       ++iterator)
  {
    (*iterator)->Render();
  }


}

void Game::Update()
{
  spatialHashing->DeleteEntity(player);
  spatialHashing->InsertEntity(player);

  for(EntityList::iterator iterator = nearyByEntities.begin(), end = nearyByEntities.end();
       iterator != end;
       ++iterator)
  {
    (*iterator)->isActive = false;
  }

  nearyByEntities.clear();
  nearyByEntities = spatialHashing->GetNearByEntities(player);

  for(EntityList::iterator iterator = nearyByEntities.begin(), end = nearyByEntities.end();
       iterator != end;
       ++iterator)
  {
    (*iterator)->isActive = true;
  }

  player->Update();

  for(EntityList::iterator iterator = entities.begin(), end = entities.end();
       iterator != end;
       ++iterator)
  {
    (*iterator)->Update();
  }
}

void Game::MouseClick(int button, int state, int x, int y) {}

void Game::MouseUpdate(int x, int y)
{
  player->SetPosition(x ,mapHeight - y ,0);
}