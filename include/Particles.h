#pragma once
#include "raylib.h"

class Particle
{
private:
    Vector2 particle_position;
    Color particle_color;
    bool visible = true; // visible, false = invisible
    int particle_size = 5;
    int particle_speed;

public:
    friend void createParticles();
    friend void drawParticles();
};

//Particle System
void createParticles(); //Create
void drawParticles();  //Update