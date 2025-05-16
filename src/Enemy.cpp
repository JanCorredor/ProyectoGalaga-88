#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include "Defines.h"
#include <vector>
#include "Player.h"
#include "Timer.h"

#include <iostream>
using namespace std;

#include "Enemy.h"

Enemy::Enemy() {
    this->enemy_radius = 64; this->enemy_alive = true; this->enemy_speed = { 5,5 }; this->angle = 0;
}

void Enemy::SpawnHorde(std::vector <Enemy>* manager, int num, int spawnId, std::vector <int> enemyId)
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

        if (enemyId[i] == 0)
        {
            newEnemy.type = Goei;
        }
        else if (enemyId[i] == 1)
        {
            newEnemy.type = Zako; //Zako
        }
        else if (enemyId[i] == 2)
        {
            newEnemy.type = Bon;
        }
        else
        {
            newEnemy.type = Bos;
        }

        manager->push_back(newEnemy);
    }
}

void Enemy::SpawnLevel1(std::vector <Enemy>* manager, int wave)
{
    //Upper Center = 0 //Upper Left //Upper Right //Center Left //Center Right //Low Left //Low Righ
    if (wave == 0)
    {
        std::vector <int> todoZako = { 1,1,1,1 };
        SpawnHorde(manager, 4, 3, todoZako); //Zako Cent Left
        SpawnHorde(manager, 4, 4, todoZako); //Zako Cent Right
    }
    else if (wave == 1)
    {
        std::vector <int> GBGBGBGBon = { 0, 2,0,2,0,2,0,2 };
        SpawnHorde(manager, 8, 5, GBGBGBGBon); //Goei Bon Goei Bon Low Left
    }
    else if (wave == 2)
    {
        std::vector <int> GBGBGBGBos = { 0, 3,0,3, 0, 3,0,3 };
        SpawnHorde(manager, 8, 6, GBGBGBGBos); //Goei Bos Goei Bos Low Right
    }
    else if (wave == 3)
    {
        std::vector <int> GBGBon = { 0, 2,0,2,0, 2,0,2 };
        SpawnHorde(manager, 8, 1, GBGBon); //Goei Bon Goei Bon Up Left
    }
    else if (wave == 4)
    {
        std::vector <int> ZBZBon = { 1, 2, 1,2, 1, 2, 1,2 };
        SpawnHorde(manager, 8, 2, ZBZBon); //Zako Bon Zako Bon Up Righ
    }
}
void Enemy::SpawnLevel2(std::vector <Enemy>* manager, int wave)
{
    //Upper Center = 0 //Upper Left //Upper Right //Center Left //Center Right //Low Left //Low Righ
    if (wave == 0)
    {
        std::vector <int> todoZako = { 1,1,1,1 };
        SpawnHorde(manager, 4, 3, todoZako); //Zako Cent Left
        SpawnHorde(manager, 4, 4, todoZako); //Zako Cent Right
    }
    else if (wave == 1)
    {
        std::vector <int> GBGBGBGBon = {0,0,0,0, 2,2,2,2};
        SpawnHorde(manager, 8, 5, GBGBGBGBon); //Goei Goei Bon Bon Low Left
    }
    else if (wave == 2)
    {
        std::vector <int> GBGBGBGBos = {0,0,0,0, 3,3,3,3};
        SpawnHorde(manager, 8, 6, GBGBGBGBos); //Goei Goei Bos Bos Low Right
    }
    else if (wave == 3)
    {
        std::vector <int> GBGBon = { 0,0,0,0, 2,2,2,2 };
        SpawnHorde(manager, 8, 1, GBGBon); //Goei Goei Bon Bon Up Left
    }
    else if (wave == 4)
    {
        std::vector <int> ZBZBon = { 1,1,1,1, 2,2,2,2};
        SpawnHorde(manager, 8, 2, ZBZBon); //Zako Bon Zako Bon Up Righ
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
    if (spawnId >= 7)
    {
        spawnId = GetRandomValue(0,6);
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
    if (spawnId == 5) //Lower Left
    {
        return { -positionAdjustments, (float)GetScreenHeight() - positionAdjustments };
    }
    if (spawnId == 6) //Lower Right
    {
        return { (float)GetScreenWidth() + positionAdjustments, (float)GetScreenHeight() - positionAdjustments };
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

Vector2 Enemy::GetFormationPositions(int x_pos, int y_pos)     //Vector2 enemiesFormationPositions[10][6];
{
    Vector2 formationPosition;

    formationPosition.x = GetScreenWidth() / 7 + (x_pos * 60); //Original: GetScreenWidth() / 10
    formationPosition.y = GetScreenHeight() / 10 + (y_pos * 75);

    this->original_position = formationPosition;
    return formationPosition;
}

void Enemy::Launch(Player* p)
{
    int speed = 5; // 10
    extern bool hardmode;
    if (enemy_color.r == WHITE.r)
    {
        if (this->type == Goei || this->type == Bon)
        {
            if (this->enemy_position.y - 100 < GetScreenHeight())
            {
                this->enemy_position.y += speed; // 10 
                this->enemy_position.x += sin(aux) * 7;
                this->aux += 0.1;
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                enemy_color = RED;
            }
        }
        else if (this->type == Zako)
        {
            float distanceLeft = abs(this->enemy_position.x - 0);
            float distanceRight = abs(this->enemy_position.x - GetScreenWidth());
            bool Left;
            if (distanceLeft <= distanceRight)
            {
                Left = true;
            }
            else
            {
                Left = false;
            }

            int radius;
            if (hardmode)
            {
                radius = 20;
            }
            else
            {
                radius = 5;
            }

            if ((this->enemy_position.y) < p->GetPositionY() - radius && this->aux == 0)
            {
                this->angle = 0;
                this->texture_angle = 0;
                this->enemy_position.y += speed; // 10 
            }
            else if ((this->texture_angle < 360 && this->texture_angle > -360) && hardmode == false)
            {
                this->aux = 1;
                if (Left)
                {
                    this->enemy_position.x += cos(this->angle) * radius;
                    this->enemy_position.y -= sin(-this->angle) * radius;
                    this->angle -= 0.1;
                    this->texture_angle -= 7.5;
                }
                else
                {
                    this->enemy_position.x -= cos(this->angle) * radius;
                    this->enemy_position.y -= sin(this->angle) * radius;
                    this->angle += 0.1;
                    this->texture_angle += 7.5;
                }
            }
            else if (hardmode) //Hardmode
            {
                this->aux = 1;

                this->enemy_position.x += cos(this->angle) * radius / 4;
                this->enemy_position.y += sin(this->angle) * radius;

                this->angle += 0.1;
                this->texture_angle += 7.5;
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                this->angle = 0;
                this->texture_angle = 0;
                this->aux = 0;
                enemy_color = RED;
            }
            else
            {
                this->enemy_position.y += speed;  
            }
        }
        else if (this->type == Bos)
        {
            if (this->enemy_position.y < p->GetPosition().y - 300 && this->aux == 0)
            {
                this->enemy_position.y += speed;  
            }
            else if (aux == 0)
            {
                atta.StartTimer(3);
                aux = 1;
            }
            else if (atta.CheckFinished() == false) //Timer 
            {
                if (p->GetPosition().x > this->enemy_position.x - 100 && p->GetPosition().x < this->enemy_position.x + 100) 
                {
                    if (p->GetAlive() == true && p->GetInmortal() == false) 
                    {
                        p->Death();
                        p->CheckDeath();
                    }
                }
            }
            else if (this->enemy_position.y >= GetScreenHeight())
            {
                this->enemy_position.y = -10;
                enemy_color = RED;
                aux = 0;
            }
            else
            {
                this->enemy_position.y += speed;
            }
        }
    }
    else if (enemy_color.g == RED.g)
    {
        MoveToInAStraightLine(original_position, 3);
        if (this->enemy_position.x == this->original_position.x && this->enemy_position.y == this->original_position.y)
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

Boss::Boss() 
{
    this->enemy_radius = 128; this->enemy_alive = false; this->enemy_speed = { 5,5 }; this->angle = 0; this->hp = 5;
}

void Boss::GetHit() 
{
    this->hp--;
    if (this->hp > 0)
    {
        SetEnemyLife(false);
    }
    else 
    {
        //Animation
    }

}

void Boss::SpawnBoss() 
{
    if (this->enemy_alive == false) 
    {
        this->enemy_alive = true;

        if (this->enemy_position.y > 1)
        {

        }
    }
}