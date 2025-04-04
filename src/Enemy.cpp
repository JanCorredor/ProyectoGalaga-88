#include "raylib.h"
#include "resource_dir.h"
#include <math.h>

#include "Enemy.h"


Enemy::Enemy() {
    this->enemy_radius = 50; this->enemy_color = RED;  this->enemy_alive = true; this->enemy_speed = 5; this->inFormation = false; this->enemy_position = {0,0};
}

void Enemy::moveToInAStraightLine(Vector2 destination)
{
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
    else if (this->enemy_position.y - destination.y < 5 && this->enemy_position.y - destination.y > -5)
    {
        if (this->enemy_position.x - destination.x < 5 && this->enemy_position.x - destination.x > -5)
        {
            this->inFormation = true;
        }
    }
    else if (this->enemy_position.x != destination.x)
    {
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

Vector2 Enemy:: FormationPositions(int fila,int columna)     //Vector2 enemiesFormationPositions[10][6];
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