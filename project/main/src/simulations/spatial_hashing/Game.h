//
// Created by Akira Lynn on 28/02/22.
//

#ifndef ALGORITHMS_GAME_H
#define ALGORITHMS_GAME_H

#include <stdlib.h>
#include <iostream>
#include "stdio.h"
#include "list"

using namespace std;

class MathC;
class SpatialHashing;
class Entity;

class Game
{
  typedef list<Entity*> EntityList;

  SpatialHashing* spatialHashing;
  Entity* player;
  EntityList entities;

  EntityList nearyByEntities;

public:
  Game();
  ~Game();

  void Render();
  void Update();
  void MouseClick(int button, int state, int x, int y);
  void MouseUpdate(int x, int y);

  int mapWidth = 600;
  int mapHeight = 600;
  int cellSize = 100;



};

#endif // ALGORITHMS_GAME_H
