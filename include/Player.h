#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

class Player {
private:
    Vector2 position;
    int radius;
    Color color;
    int score;
    int lives;
public:
    //Constructores
    Player();
    Player(Vector2 _position, int _radius, Color _color, int _score, int _lives);

    //Getters y Setters
    Vector2 GetPosition();
    void SetPosition(Vector2 newVector);
    float GetPositionX();
    void SetPositionX(float newPositionX);
    float GetPositionY();
    void SetPositionY(float newPositionY);

    int GetRadius();
    void SetRadius(int newRadius);

    Color GetColor();
    void SetColor(Color newColor);

    int GetScore();
    void SetScore(int newScore);

    int GetLives();
    void SetLives(int newLives);

    //Funciones
    void SumScore(int n);

    void Shoot();

    void Death();



};