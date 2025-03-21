#pragma once

#include "raylib.h"

class Particle
{
private:
    Vector2 particle_position = { 0,0 };
    Color particle_color = BLACK;
    bool visible = true; // visible, false = invisible
    int particle_size = 5;
    int particle_speed = 0;
public:
    friend void createParticles();
    friend void drawParticles();
};


//Particle System
void createParticles(); //Create
void drawParticles();  //Update