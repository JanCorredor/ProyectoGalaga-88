#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include "Defines.h"
#include <vector>
#include "Player.h"

#include "Enemy.h"

#include "loadResources.h"

ResourceManager r;

Enemy::Enemy() {
    this->enemy_radius = 50; this->enemy_color = RED;  this->enemy_alive = true; this->enemy_speed = { 5,5 }; this->angle = 0;
}

void Enemy::spawnHorde(std::vector <Enemy>* manager, int num, int spawnId)
{
    for (int i = 0; i < num; i++)
    {
        float space_manager = (1+i*0.2);
        Vector2 spaceBetween = { space_manager * startingPositions(spawnId).x, space_manager *startingPositions(spawnId).y };
        Enemy newEnemy;
        newEnemy.setEnemyPosition(newEnemy.startingPositions(spawnId).x + spaceBetween.x, newEnemy.startingPositions(spawnId).y + spaceBetween.y);

        if (i == 0)
        {
            newEnemy.enemySprite = r.Goei_0;
        }
        else if (i != num -1)
        {
            newEnemy.enemySprite = r.Zako;
        }
        else
        {
            newEnemy.enemySprite = r.Bon;
        }

        manager->push_back(newEnemy);
    }
}

void Enemy::moveToInAStraightLine(Vector2 destination, int positionId)
{
    float distanceX = abs(this->enemy_position.x - destination.x);
    float distanceY = abs(this->enemy_position.y - destination.y);

    if (distanceY < tolerance)
    {
        if (distanceX < tolerance)
        {
            this->inPosition[positionId] = true;
        }
    }

    if (this->enemy_position.x != destination.x)
    {
        if (distanceX < tolerance)
        {
            this->enemy_speed.x = distanceX;
        }

        if (this->enemy_position.x - destination.x > 0)
        {
            this->enemy_position.x -= this->enemy_speed.x;

        }
        else if(this->enemy_position.x - destination.x < 0)
        {
            this->enemy_position.x += this->enemy_speed.x;
        }
        this->enemy_speed.x = 5;
    }

    if (this->enemy_position.y != destination.y)
    {
        if (distanceY < tolerance)
        {
            this->enemy_speed.y = distanceY;
        }

        if (this->enemy_position.y - destination.y > 0)
        {
            this->enemy_position.y -= this->enemy_speed.y;

        }
        else if (this->enemy_position.y - destination.y < 0)
        {
            this->enemy_position.y += this->enemy_speed.y;
        }
        this->enemy_speed.y = 5;
    }
}

Vector2 Enemy::startingPositions(int spawnId)
{
    //Debug Purposes && FailSave
    if (spawnId >= 7)
    {
        spawnId = GetRandomValue(0,6);
    }

    float centroVertical = GetScreenHeight() / 2;
    float centroHorizonal = GetScreenWidth() / 2;

    if (spawnId == 0) //Upper Left
    {
        return { -positionAdjustments, -100 };
    }
    if (spawnId == 1) //Upper Center
    {
        return { centroHorizonal, -100};
    }
    if (spawnId == 2) //Upper Right
    {
        return { (float)GetScreenWidth()+positionAdjustments, - 100 };
    }
    if (spawnId == 3) //Center Left
    {
        return {-positionAdjustments , centroVertical};
    }
    if (spawnId == 4) //Center Right
    {
        return { (float)GetScreenWidth() + positionAdjustments, centroVertical };
    }
    if (spawnId == 5) //Lower Left
    {
        return { -positionAdjustments, (float)GetScreenHeight()-positionAdjustments };
    }
    if (spawnId == 6) //Lower Right
    {
        return { (float)GetScreenWidth() + positionAdjustments, (float)GetScreenHeight() - positionAdjustments };
    }
}

Vector2 Enemy::semiCirclePoints()
{
    int verticalCirclePosition = GetScreenHeight() / 2;
    Vector2 LeftPoint = { positionAdjustments, verticalCirclePosition};
    Vector2 RightPoint = {GetScreenWidth() - positionAdjustments, verticalCirclePosition};

    float distanceLeft = abs(this->enemy_position.x - LeftPoint.x);
    float distanceRight = abs(this->enemy_position.x - RightPoint.x);

    if (distanceLeft <= distanceRight)
    {
        this->angle = 90;
        return LeftPoint;
    }
    else
    {
        this->angle = 90;
        return RightPoint;
    }
}

void Enemy::semiCircleMovement()
{
    int radius = 15;
    int verticalCirclePosition = GetScreenHeight() / 2;

    float distanceLeft = abs(this->enemy_position.x - 0);
    float distanceRight = abs(this->enemy_position.x - GetScreenWidth());

    if (distanceLeft <= distanceRight)
    {
        this->enemy_position.x -= cos(this->angle) * radius;
        this->enemy_position.y += sin(this->angle) * radius;
    }
    else
    {
        this->enemy_position.x += cos(this->angle) * radius;
        this->enemy_position.y += sin(this->angle) * radius;;
    }

    this->angle += 0.1;

    if ((positionAdjustments + radius) - this->enemy_position.x < tolerance || this->enemy_position.x - (GetScreenWidth() - (positionAdjustments + radius)) < tolerance)
    {
        if (this->enemy_position.y - verticalCirclePosition <= 1)
        {
            this->inPosition[1] = true;
        }
    }

}

Vector2 Enemy:: formationPositions(int fila,int columna)     //Vector2 enemiesFormationPositions[10][6];
{
    Vector2 formationPosition;
    formationPosition.x = GetScreenWidth() / 7 + (fila * 60); //Original: GetScreenWidth() / 10
    formationPosition.y = GetScreenHeight() / 10 + (columna * 75);
    return formationPosition;
}

void Enemy::shoot(std::vector <Bullet>* bulletManager, Player player)
{
    Bullet newBullet;

    if (this->enemy_alive == true)
    {
        newBullet.bullet_position = this->enemy_position;

        if (newBullet.bullet_position.x == player.GetPosition().x)
        {
            newBullet.bullet_color = RED;
        }
        else if (newBullet.bullet_position.x > player.GetPosition().x)
        {
            newBullet.bullet_color = ORANGE; // MINUS
        }
        else
        {
            newBullet.bullet_color = PURPLE; // PLUS
        }

        newBullet.bullet_radius = 10;
        bulletManager->push_back(newBullet);
    }

}


Vector2 Enemy::getEnemyPosition() { return enemy_position; }
int Enemy::getEnemyRadius() { return enemy_radius; }
bool Enemy::isEnemyAlive() { return enemy_alive; }

void Enemy::setEnemyPosition(Vector2 v2) { this->enemy_position = v2; }
void Enemy::setEnemyPosition(int x, int y) { this->enemy_position.x = x; this->enemy_position.y = y; }
void Enemy::setEnemyRadius(int rad) { this->enemy_radius = rad; };
void Enemy::setEnemyLife(bool alive) { this->enemy_alive = alive; };