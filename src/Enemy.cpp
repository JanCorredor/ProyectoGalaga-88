#include "raylib.h"
#include "resource_dir.h"
#include <math.h>

#include "Enemy.h"


Enemy::Enemy() {
    this->enemy_radius = 50; this->enemy_color = RED;  this->enemy_alive = true; this->enemy_speed = 5; this->angle = 0;
}

void Enemy::moveToInAStraightLine(Vector2 destination)
{
    int tolerance = 5;

    if (this->enemy_position.y - destination.y < tolerance && this->enemy_position.y - destination.y > -tolerance)
    {
        if (this->enemy_position.x - destination.x < tolerance && this->enemy_position.x - destination.x > -tolerance)
        {
            this->inPosition[0] = true;
            this->inPosition[2] = true;
        }
    }

    if (this->enemy_position.y != destination.y && this->enemy_position.x != destination.x)
    {
        if (this->enemy_position.x - destination.x > 0)
        {
            this->enemy_position.x -= this->enemy_speed;
        }
        else if (this->enemy_position.x - destination.x < 0)
        {
            this->enemy_position.x += this->enemy_speed;
        }
        if (this->enemy_position.y - destination.y > 0)
        {
            this->enemy_position.y -= this->enemy_speed;
        }
        else if (this->enemy_position.y - destination.y < 0)
        {
            this->enemy_position.y += this->enemy_speed;
        }
    }
    else if (this->enemy_position.x != destination.x)
    {
        //if (this->enemy_position.x - destination.x < tolerance)
        //{
        //    this->enemy_speed = this->enemy_position.x - destination.x;
        //}
        if (this->enemy_position.x - destination.x > 0)
        {
            this->enemy_position.x -= this->enemy_speed;

        }
        else if(this->enemy_position.x - destination.x < 0)
        {
            this->enemy_position.x += this->enemy_speed;
        }
    }
    else if (this->enemy_position.y != destination.y)
    {
        //if (this->enemy_position.y - destination.y < tolerance)
        //{
        //    this->enemy_speed = this->enemy_position.y - destination.y;
        //}
        if (this->enemy_position.y - destination.y > 0)
        {
            this->enemy_position.y -= this->enemy_speed;

        }
        else if (this->enemy_position.y - destination.y < 0)
        {
            this->enemy_position.y += this->enemy_speed;
        }
    }
}

float ajuste = 100;
Vector2 Enemy::startingPositions(int positionId)
{
    //Debug Purposes && FailSave
    if (positionId >= 7)
    {
        positionId = GetRandomValue(0,6);
    }

    float centroVertical = GetScreenHeight() / 2;
    float centroHorizonal = GetScreenWidth() / 2;

    if (positionId == 0) //Upper Left
    {
        return { -ajuste, -100 };
    }
    if (positionId == 1) //Upper Center
    {
        return { centroHorizonal, -100};
    }
    if (positionId == 2) //Upper Right
    {
        return { GetScreenWidth()+ajuste, - 100 };
    }
    if (positionId == 3) //Center Left
    {
        return {-ajuste , centroVertical};
    }
    if (positionId == 4) //Center Right
    {
        return { GetScreenWidth() + ajuste, centroVertical };
    }
    if (positionId == 5) //Lower Left
    {
        return { -ajuste, GetScreenHeight()-ajuste };
    }
    if (positionId == 6) //Lower Right
    {
        return { GetScreenWidth() + ajuste, GetScreenHeight() - ajuste };
    }
}

Vector2 Enemy::semiCirclePoints()
{
    Vector2 LeftPoint = { ajuste, GetScreenHeight()/3};
    Vector2 RightPoint = {GetScreenWidth() - ajuste, GetScreenHeight() / 3};

    float distanceLeft = abs(this->enemy_position.x - LeftPoint.x);
    float distanceRight = abs(this->enemy_position.x - RightPoint.x);

    if (distanceLeft <= distanceRight)
    {
        this->angle = 90;
        return LeftPoint;
    }
    else
    {
        return RightPoint;
    }
}

void Enemy::semiCircleMovement()
{
    int radius = 10;
    
    float distanceLeft = abs(this->enemy_position.x - 0);
    float distanceRight = abs(this->enemy_position.x - GetScreenWidth());

    if (distanceLeft <= distanceRight)
    {
        this->enemy_position.x -= cos(this->angle) * radius;
        this->enemy_position.y += sin(this->angle) * radius;;
    }
    else
    {
        this->enemy_position.x += cos(this->angle) * radius;
        this->enemy_position.y += sin(this->angle) * radius;;
    }


    this->angle += 0.1;
    if (this->enemy_position.y - GetScreenHeight() / 3 <= 1)
    {
        this->inPosition[1] = true;
        this->inPosition[2] = false;
    }
}

Vector2 Enemy:: formationPositions(int fila,int columna)     //Vector2 enemiesFormationPositions[10][6];
{
    Vector2 formationPosition;
    formationPosition.x = (GetScreenWidth() / 10) + (fila * 60);
    formationPosition.y = GetScreenHeight() / 10 + (columna * 75);
    return formationPosition;
}

Vector2 Enemy::getEnemyPosition() { return enemy_position; }
int Enemy::getEnemyRadius() { return enemy_radius; }
bool Enemy::isEnemyAlive() { return enemy_alive; }

void Enemy::setEnemyPosition(Vector2 v2) { this->enemy_position = v2; }
void Enemy::setEnemyPosition(int x, int y) { this->enemy_position.x = x; this->enemy_position.y = y; }
void Enemy::setEnemyRadius(int rad) { this->enemy_radius = rad; };
void Enemy::setEnemyLife(bool alive) { this->enemy_alive = alive; };