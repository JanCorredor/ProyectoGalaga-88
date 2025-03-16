#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <math.h>

#include <vector>

class Player {
private:
    Vector2 position;
    int radius;
    Color color;
    int score;
    int lifes;
public:
    //Constructores
    Player() 
    {
    
        this->radius = 25;
        this->position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() * 9 / 10 };
    
    }
    
    //Getters y Setters
    Vector2 GetPosition() { return this->position; }
    void SetPosition(Vector2 newVector) { this->position = newVector; }

    int GetRadius() { return this->radius; }
    void SetRadius(int newRadius) { this->radius = newRadius; }

    Color GetColor() { return this->color; }
    void SetColor(Color newColor) { this->color = newColor; }

    int GetScore() { return this->score; }
    void SetScore(int newScore) { this->score = newScore; }

    int GetLifes() { return this->lifes; }
    void SetLifes(int newLifes) { this->lifes = newLifes; }

    //Funciones
    void SumScore(int n) 
    {
        this->score += n;
    }

    void Shoot() 
    {



    }

    void Death() 
    {
    
    
    
    }



};