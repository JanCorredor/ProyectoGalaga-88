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
    friend void CreateParticles();
    friend void DrawParticles();
};

//Particle System
void CreateParticles(); //Create particles
void DrawParticles();  //Update