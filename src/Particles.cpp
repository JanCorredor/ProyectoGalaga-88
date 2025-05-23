#include "Particles.h"

#define MAXPARTICLES 200  //Particles in screen
Particle particles[MAXPARTICLES];

void CreateParticles()
{
    Color particle_colors[6] = { GREEN, SKYBLUE, BLUE, RED, WHITE, LIGHTGRAY }; //Particles Possible Colors

    for (int i = 0; i < MAXPARTICLES; i++) //Create
    {
        particles[i].particle_position = { (float)GetRandomValue(GetScreenWidth() / 200, GetScreenWidth() * 199 / 200), (float)GetRandomValue(GetScreenHeight() / 200, GetScreenHeight() * 199 / 200) }; //Random position in screen
        particles[i].particle_color = particle_colors[GetRandomValue(0, 5)]; // Pick Random Color
        particles[i].particle_speed = GetRandomValue(1, 3);
    }
}

void DrawParticles()
{
    Color particle_colors[6] = { GREEN, SKYBLUE, BLUE, RED, WHITE, LIGHTGRAY }; //Particles Possible Colors

    for (int i = 0; i < MAXPARTICLES; i++) //Update
    {
        particles[i].particle_position.y += particles[i].particle_speed;

        if (GetRandomValue(0, 100) == 0) // Particles Blink
        {
            particles[i].visible = false; // Invisible
        }
        else if (GetRandomValue(0, 50) == 50)
        {
            particles[i].visible = true; // Visible
        }

        if ((particles[i].particle_position.y) >= GetScreenHeight()) // Restart 
        {
            particles[i].particle_position = { (float)GetRandomValue(GetScreenWidth() / 200, GetScreenWidth() * 199 / 200), (float)GetRandomValue(0, 5) };// Position
            particles[i].particle_color = particle_colors[GetRandomValue(0, 5)];                                                                       //Color
            particles[i].particle_speed = GetRandomValue(1, 3);                                                                                      //Speed
        }

    }
    for (int i = 0; i < MAXPARTICLES; i++) //Draw
    {
        if (particles[i].visible == true) //Draw only if visible
        {
            extern bool hardmode;
            if (hardmode) { particles[i].particle_color = RED; }                                                                                       //Red
            DrawRectangle(particles[i].particle_position.x, particles[i].particle_position.y, particles[i].particle_size, particles[i].particle_size, particles[i].particle_color);
        }
    }
}