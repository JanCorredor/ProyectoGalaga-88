#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>
#include "Timer.h"
#include "Defines.h"

class Player {
private:
    Vector2 position;
    int radius = 25;
    Color color;
    int score = 0;
    int lives = 2;
    bool alive = true;
    bool inmortal = false;
    Timer deathTimer;
public:
    //Public Variables
    int aux;
    std::vector<Bullet>* bulletVector;
    
    //Constructors
    Player();
    Player(Vector2 _position, int _radius, Color _color, int _score, int _lives);

    //Getters & Setters
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
    
    bool GetInmortal();
    void SetInmortal(bool newInmortal);

    bool GetAlive();
    void SetAlive(bool newAlive);

    Timer GetTimer();
    void SetTimer(Timer newTimer);

    //Functions
    void SumScore(int n);

    void Move();
    void Shoot(std::vector <Bullet>* playerbullets);

    void Death();
    bool CheckDeath();
    void ToggleInmortal(bool state);
};