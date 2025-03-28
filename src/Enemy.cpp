#include "raylib.h"
#include "resource_dir.h"
#include <math.h>

#include "Enemy.h"


Enemy::Enemy() { this->enemy_radius = 50; }

Vector2 Enemy:: FormationPositions(int fila,int columna)     //Vector2 enemiesFormationPositions[10][6];
{
    Vector2 formationPosition;
    formationPosition.y = GetScreenHeight() / 10 + (columna * 75);
    formationPosition.x = (GetScreenWidth() / 10) + (fila * 60);
    return formationPosition;
}

Vector2 Enemy::getEnemyPosition() { return enemy_position; }
int Enemy::getEnemyRadius() { return enemy_radius; }
bool Enemy::isEnemyAlive() { return enemy_alive; }

void Enemy::setEnemyPosition(Vector2 v2) { this->enemy_position = v2; }
void Enemy::setEnemyPosition(int x, int y) { this->enemy_position.x = x; this->enemy_position.y = y; }
void Enemy::setEnemyRadius(int rad) { this->enemy_radius = rad; };
void Enemy::setEnemyLife(bool alive) { this->enemy_alive = alive; };