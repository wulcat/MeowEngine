//
// Created by Akira Lynn on 28/02/22.
//
#pragma once

#ifndef TEST_ENTITY_H
#define TEST_ENTITY_H

class Vector3;
class Text;

class Entity {
public:
  Entity(float x, float y, float z, float scale);
  ~Entity();

  void Render();
  void Update();

  Vector3 GetPosition();
  float GetSize();

  void SetPosition(float x, float y, float z);
  void SetSize(float size);

  bool isActive;
  int cellID;
private:
  Vector3* position;
  float size;
};

#endif // TEST_ENTITY_H
