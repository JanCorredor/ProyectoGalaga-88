#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include "Defines.h"
#include <vector>
#include "Player.h"

#define tolerance 5
#define positionAdjustments 100

typedef enum enemyType
{
    Goei = 0, Zako, Bon, Bos
} enemyType;

class Enemy
{
protected:
    Vector2 enemy_position;
    int enemy_radius;
    Color enemy_color = WHITE;
    bool enemy_alive;
    Vector2 enemy_speed;
    float angle;

public:
    Enemy();
    bool inPosition[4] = {false,false,false, false}; //CircleStart, CricleEnd, Formation, Assault
    Vector2 original_position = { 0,0 };
    int aux = 0;

    enemyType type;
    float texture_angle = 0;
    Vector2 enemy_texture_position;

    void spawnHorde(std::vector <Enemy>* manager, int num, int spawnId);

    void moveToInAStraightLine(Vector2 destination, int positionId);
    Vector2 startingPositions(int spawnId);
    Vector2 semiCirclePoints();
    void semiCircleMovement();
    Vector2 formationPositions(int fila, int columna);
    void Launch(const Player p);


    void shoot(std::vector <Bullet>* bulletManager, Player player);

    Vector2 getEnemyPosition();
    int getEnemyRadius();
    bool isEnemyAlive();
    float getAngle();

    void setEnemyPosition(Vector2 v2);
    void setEnemyPosition(int x, int y);
    void setEnemyRadius(int rad);
    void setEnemyLife(bool alive);

};