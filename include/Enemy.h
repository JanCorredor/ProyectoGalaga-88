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
    float aux = 0;

    Color DEAD = { 230, 41, 55, 255 };
    enemyType type;
    float texture_angle = 0;
    Vector2 enemy_texture_position;
    Timer atta;


    void SpawnHorde(std::vector <Enemy>* manager, int num, int spawnId, std::vector <int> enemyId);
    void SpawnLevel1(std::vector <Enemy>* manager, int wave);
    void SpawnLevel2(std::vector <Enemy>* manager, int wave);


    void MoveToInAStraightLine(Vector2 destination, int positionId);
    Vector2 StartingPositions(int spawnId);
    Vector2 GetSemiCirclePoints();
    void SemiCircleMovement();
    Vector2 GetFormationPositions(int x_pos, int y_pos);
    void Launch(Player* p);


    void Shoot(std::vector <Bullet>* bulletManager, Player player);

    Vector2 GetEnemyPosition();
    int GetEnemyRadius();
    bool IsEnemyAlive();
    float GetAngle();

    void SetEnemyPosition(Vector2 v2);
    void SetEnemyPosition(int x, int y);
    void SetEnemyRadius(int rad);
    void SetEnemyLife(bool alive);
};

class Boss : public Enemy 
{
private:
    int hp;
    bool isSpawned;
    bool isHit;
    Timer timerHitAnimation;
public:
    Boss();

    int GetHP();
    void SetHP(int newHP);
    void GetHit();
    bool GetIsHit();
    void SetIsHit(bool newHit);
    bool CheckIsHit();
    void SpawnBoss();
};