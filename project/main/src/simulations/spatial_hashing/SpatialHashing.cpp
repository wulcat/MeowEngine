#include "SpatialHashing.h"
#include "Entity.h"
#include "Vector3.h"

SpatialHashing::SpatialHashing(int mapWidth, int mapHeight, int cellSize)
{
  mapWidth = mapWidth;
  mapHeight = mapHeight;
  cellSize = cellSize;

  columns = mapWidth / cellSize;
  rows = mapHeight / cellSize;
  columnWidth = mapWidth / cellSize;
  conversionFactor = 1 / (float)cellSize;

  for(int i = 0 ; i < columns * rows ; i++)
  {
    cellTable[i] = EntityList();
  }
}

void SpatialHashing::ClearTable()
{
  cellTable.clear();
  for(int i = 0 ; i < columns * rows ; i++)
  {
    cellTable[i] = EntityList();
  }
}

int SpatialHashing::GetKeyFromTable(Vector3 position)
{
  return floor(position.x * conversionFactor) + floor(position.y * conversionFactor) * columnWidth;
}

void SpatialHashing::InsertEntity(Entity* entity)
{
  int cellID = GetKeyFromTable(entity->GetPosition());

  entity->cellID = cellID;
  cellTable[cellID].push_back(entity);
}

void SpatialHashing::DeleteEntity(Entity* entity)
{
  int cellID = GetKeyFromTable(entity->GetPosition());

  std::remove(cellTable[cellID].begin(), cellTable[cellID].end(), entity);
}

SpatialHashing::EntityList SpatialHashing::GetNearByEntities(Entity* entity)
{
  Vector3 position = entity->GetPosition();
  float scale = entity->GetSize();

  Vector3 minPosition = Vector3();
  Vector3 maxPosition = Vector3();

  // Calculate Bounds to retreive based on player entity size
  minPosition.x = position.x - scale;
  minPosition.y = position.y - scale;

  maxPosition.x = position.x + scale;
  maxPosition.y = position.y + scale;

  EntityList fetchedEntities;
  EntityList entityList = cellTable[GetKeyFromTable(position)];
  fetchedEntities.assign(entityList.begin(), entityList.end());

  entityList = cellTable[GetKeyFromTable(minPosition)];
  fetchedEntities.assign(entityList.begin(), entityList.end());

  entityList = cellTable[GetKeyFromTable(maxPosition)];
  fetchedEntities.assign(entityList.begin(), entityList.end());

  entityList = cellTable[GetKeyFromTable(Vector3(minPosition.x, maxPosition.y, 0))];
  fetchedEntities.assign(entityList.begin(), entityList.end());

  entityList = cellTable[GetKeyFromTable(Vector3(maxPosition.x, minPosition.y, 0))];
  fetchedEntities.assign(entityList.begin(), entityList.end());

  return fetchedEntities;
//  int cellID = GetKeysFromTable(entity);
//  EntityList cloneEntities;
//  EntityList fetchedEntities = cellTable[cellID];
//
//  for(EntityList::const_iterator iterator = fetchedEntities.begin(),
//                                  end = fetchedEntities.end();
//       iterator != end;
//       ++iterator)
//  {
//    cloneEntities.push_back(*iterator);
//  }

//  return fetchedEntities;
}