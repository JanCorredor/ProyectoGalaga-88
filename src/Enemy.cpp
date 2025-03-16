#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <math.h>

#include <vector>

enum EnemyType {ZAKO,BOOS,BON,GOEI,CHICK};

class Enemy {
private:
    Vector2 enemy_position;
    int enemy_radius;
    Color enemy_color;
    bool enemy_alive;
public:
    //Constructores
    Enemy()
    {
        enemy_alive = true;
    }
    Enemy(Vector2 _enemy_position,int _enemy_radius, Color _enemy_color, bool _enemy_alive)
    {
        this->enemy_position = _enemy_position;
        this->enemy_radius = _enemy_radius;
        this->enemy_color = _enemy_color;
        this->enemy_alive = _enemy_alive;
    }

    //Getters y Setters
    Vector2 GetEnemyPosition() { return this->enemy_position; }
    void SetEnemyPosition(Vector2 newEnemyPosition) { this->enemy_position = newEnemyPosition; }

    int GetEnemyRadius() { return this->enemy_radius; }
    void SetEnemyRadius(int newEnemyRadius) { this->enemy_radius = newEnemyRadius; }

    Color GetEnemyColor() { return this->enemy_color; }
    void SetEnemyColor(Color newEnemyColor) { this->enemy_color = newEnemyColor; }

    bool GetAlive() { return this->enemy_alive; }
    void SetScore(bool newEnemyAlive) { this->enemy_alive = newEnemyAlive; }

    void Death() 
    {
    
        //Animacion de morir
    
    }
    
    void Shoot() 
    {
    
      
    
    }

    void EnemyAction() 
    {
    
    
    
    }

    void FormationMovement() 
    {
    
    
    
    }

};