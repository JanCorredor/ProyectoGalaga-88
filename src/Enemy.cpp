#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include "Defines.h"
#include <vector>
#include "Player.h"

#include "Enemy.h"

Enemy::Enemy() {
    this->enemy_radius = 64; this->enemy_alive = true; this->enemy_speed = { 5,5 }; this->angle = 0;
}

void Enemy::SpawnHorde(std::vector <Enemy>* manager, int num, int spawnId)
{
    for (int i = 0; i < num; i++)
    {
        Vector2 spaceBetween;
        if (spawnId <= 2) //Up
        {
            spaceBetween = { StartingPositions(spawnId).x * 0, StartingPositions(spawnId).y * i / 10}; // -
        }
        else if (spawnId == 3) //Center Left
        {
            spaceBetween = { StartingPositions(spawnId).x * i / 10, StartingPositions(spawnId).y * 0 }; // -
        }
        else if (spawnId == 4) //Right
        {
            spaceBetween = { StartingPositions(spawnId).x * i / 10, StartingPositions(spawnId).y * 0 };
        }
        else //Lower
        {
            spaceBetween = { StartingPositions(spawnId).x * 0, StartingPositions(spawnId).y * i / 10 };
        }

        Enemy newEnemy;
        newEnemy.SetEnemyPosition(newEnemy.StartingPositions(spawnId).x + spaceBetween.x, newEnemy.StartingPositions(spawnId).y + spaceBetween.y);

        if (i == 0)
        {
            newEnemy.type = Goei;
        }
        else if (i != num -1)
        {
            newEnemy.type = Zako;
        }
        else
        {
            newEnemy.type = Bon;
        }

        manager->push_back(newEnemy);
    }
}

void Enemy::MoveToInAStraightLine(Vector2 destination, int positionId)
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

Vector2 Enemy::StartingPositions(int spawnId)
{
    //Debug Purposes && FailSave
    if (spawnId >= 5)
    {
        spawnId = GetRandomValue(0,4);
    }

    float centroVertical = GetScreenHeight() / 2;
    float centroHorizonal = GetScreenWidth() / 2;

    if (spawnId == 0) //Upper Center
    {
        return { centroHorizonal, -100 }; 
        
    }
    if (spawnId == 1) //Upper Left
    {
        return { -positionAdjustments, -100 };
    }
    if (spawnId == 2) //Upper Right
    {
        return { (float)GetScreenWidth()+positionAdjustments, - 100 };
    }
    if (spawnId == 3) //Center Left
    {
        texture_angle = 90;
        return {-positionAdjustments , centroVertical};
    }
    if (spawnId == 4) //Center Right
    {
        texture_angle = -90;
        return { (float)GetScreenWidth() + positionAdjustments, centroVertical };
    }
}

Vector2 Enemy::GetSemiCirclePoints()
{
    int verticalCirclePosition = GetScreenHeight() / 2;
    Vector2 LeftPoint = { positionAdjustments, verticalCirclePosition};
    Vector2 RightPoint = {GetScreenWidth() - positionAdjustments, verticalCirclePosition};

    float distanceLeft = abs(this->enemy_position.x - LeftPoint.x);
    float distanceRight = abs(this->enemy_position.x - RightPoint.x);

    if (distanceLeft <= distanceRight)
    {
        this->angle = 90;
        texture_angle = 180;
        return LeftPoint;
    }
    else
    {
        this->angle = 90;
        texture_angle = 180;
        return RightPoint;
    }
}

void Enemy::SemiCircleMovement()
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
    texture_angle += 7.5;

    if ((positionAdjustments + radius) - this->enemy_position.x < tolerance || this->enemy_position.x - (GetScreenWidth() - (positionAdjustments + radius)) < tolerance)
    {
        if (this->enemy_position.y - verticalCirclePosition <= 1)
        {
            this->inPosition[1] = true;
        }
    }

}

Vector2 Enemy::GetFormationPositions(int fila, int columna)     //Vector2 enemiesFormationPositions[10][6];
{
    Vector2 formationPosition;

    formationPosition.x = GetScreenWidth() / 7 + (fila * 60); //Original: GetScreenWidth() / 10
    formationPosition.y = GetScreenHeight() / 10 + (columna * 75);

    this->original_position = formationPosition;
    return formationPosition;
}

void Enemy::Launch(Player p)
{
    int speed = 10; // 10
    extern bool hardmode;
    if (enemy_color.r == WHITE.r)
    {
        if (this->type == Goei)
        {
            if (this->enemy_position.y - 100 < GetScreenHeight())
            {
                this->enemy_position.y += 10; // 10 
                this->enemy_position.x += 5;
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                enemy_color = RED;
            }
        }
        if (this->type == Zako)
        {
            int radius;
            if (hardmode)
            {
                radius = 20;
            }
            else
            {
                radius = 10;
            }

            this->angle = 0;
            if ((this->enemy_position.y + 2 * radius) < p.GetPosition().y && this->aux == 0)
            {
                this->enemy_position.y += speed; // 10 
            }
            else if (this->angle < 3.6/5 && hardmode == false)
            {
                this->aux = 1;

                this->enemy_position.x += cos(this->angle + 270) * radius / 2;
                this->enemy_position.y -= sin(this->angle + 270) * radius;

                this->angle += 0.1 / 5;
                this->texture_angle += 7.5 / 5;
            }
            else if (this->angle < 36 && hardmode) //Hardmode
            {
                this->aux = 1;
                int rd = GetRandomValue(0, 1);

                if (rd == 0)
                {
                    this->enemy_position.x -= cos(this->angle) * radius / 4;
                }
                else
                {
                    this->enemy_position.x += cos(this->angle) * radius / 4;
                }
                this->enemy_position.y += sin(this->angle) * radius;

                this->angle += 0.1;
                this->texture_angle += 7.5;
            }
            //else if (this->enemy_position.y >= GetScreenHeight() && this->angle >= 36)
            //{
            //    this->enemy_position.y = -10;
            //    this->angle = 0;
            //    this->texture_angle = 0;
            //    this->aux = 0;
            //    enemy_color = RED;
            //}
            //else
            //{
            //    this->enemy_position.y += speed; 
            //}

        }
        if (this->type == Bon)
        {
            if (this->enemy_position.y - 100 < GetScreenHeight())
            {
                this->enemy_position.y += 10; // 10 
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                enemy_color = RED;
            }
        }
        if (this->type == Bos)
        {
            if (this->enemy_position.y - 100 < GetScreenHeight())
            {
                this->enemy_position.y += 10; // 10 
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                enemy_color = RED;
            }
        }
    }
    else if (enemy_color.g == RED.g)
    {
        MoveToInAStraightLine(original_position, 3);
        if (this->enemy_position.y == original_position.y)
        {
            this->inPosition[3] = false;
            this->enemy_color = WHITE;
        }
    }
}


void Enemy::Shoot(std::vector <Bullet>* bulletManager, Player player)
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


Vector2 Enemy::GetEnemyPosition() { return enemy_position; }
int Enemy::GetEnemyRadius() { return enemy_radius; }
bool Enemy::IsEnemyAlive() { return enemy_alive; }
float Enemy::GetAngle() { return angle; }

void Enemy::SetEnemyPosition(Vector2 v2) { this->enemy_position = v2; }
void Enemy::SetEnemyPosition(int x, int y) { this->enemy_position.x = x; this->enemy_position.y = y; }
void Enemy::SetEnemyRadius(int rad) { this->enemy_radius = rad; };
void Enemy::SetEnemyLife(bool alive) { this->enemy_alive = alive; };