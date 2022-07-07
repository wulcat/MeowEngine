#pragma once

#include <iostream>
#include <math.h>

#include "list"
#include "unordered_map"

using namespace std;

class Entity;
class Vector3;

class SpatialHashing 
{
  typedef list<Entity*> EntityList;
private:
  int mapWidth;
  int mapHeight;
  int cellSize;

  float columnWidth;
  float conversionFactor;

  unordered_map<int, EntityList> cellTable;

  int GetKeyFromTable(Vector3 position);
public:
  SpatialHashing(int mapWidth, int mapHeight, int cellSize);

  void ClearTable();
  void InsertEntity(Entity* entity);
  void DeleteEntity(Entity* entity);

  EntityList GetNearByEntities(Entity* entity);

  int columns;
  int rows;
};