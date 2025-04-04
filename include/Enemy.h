#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

#define tolerance 5
#define positionAdjustments 100

class Enemy
{
protected:
    Vector2 enemy_position;
    int enemy_radius;
    Color enemy_color;
    bool enemy_alive;
    Vector2 enemy_speed;
    float angle;
public:
    Enemy();
    bool inPosition[3] = {false,false,false}; //CircleStart, CricleEnd, Formation

    void spawnHorde(std::vector <Enemy>* manager, int num, int spawnId);

    void moveToInAStraightLine(Vector2 destination, int positionId);
    Vector2 startingPositions(int spawnId);
    Vector2 semiCirclePoints();
    void semiCircleMovement();
    Vector2 formationPositions(int fila, int columna);


    Vector2 getEnemyPosition();
    int getEnemyRadius();
    bool isEnemyAlive();

    void setEnemyPosition(Vector2 v2);
    void setEnemyPosition(int x, int y);
    void setEnemyRadius(int rad);
    void setEnemyLife(bool alive);

};