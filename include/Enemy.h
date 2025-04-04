#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

#define MAXENEMIES 60

class Enemy
{
protected:
    Vector2 enemy_position;
    int enemy_radius;
    Color enemy_color;
    bool enemy_alive;
    int enemy_speed;
public:
    Enemy();
    bool inFormation;

    void moveToInAStraightLine(Vector2 destination);
    Vector2 FormationPositions(int fila, int columna);


    Vector2 getEnemyPosition();
    int getEnemyRadius();
    bool isEnemyAlive();

    void setEnemyPosition(Vector2 v2);
    void setEnemyPosition(int x, int y);
    void setEnemyRadius(int rad);
    void setEnemyLife(bool alive);

};