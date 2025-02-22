/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

//-------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Types and Structures Definitions
//-------------------------------------------------------------------------------------
typedef enum GameScreen 
{ 
	LOGO = 0, TITLE, GAMEPLAY, ENDING 
} GameScreen;

//Title + Gameplay
typedef struct {
    Vector2 particle_position;
    Color particle_color;
    bool visible; // 1 visible 0 invisible
    int particle_size;
    int particle_speed;
} Particle;

//Gameplay
typedef struct Player {
    Vector2 position;
    int radius;
    Color color;
} Player;

typedef struct Bullet {
    Vector2 bullet_position;
    int bullet_radius;
    Color bullet_color;
} Bullet;



//-------------------------------------------------------------------------------------
// Global Variables 
//-------------------------------------------------------------------------------------
static Player player = { 0 };

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

//Particle System
#define MAXPARTICLES 100  //Particles in screen
Particle particles[MAXPARTICLES];
void createParticles(); //Create
void drawParticles();  //Update

//Disparar
#define MAXPLAYERBULLETS 100  // MAX PLAYER BULLETS IN the SCREEN at the same time
Bullet playerbullets[MAXPLAYERBULLETS];
int player_bullet_counter = -1;
void ShootBullet();  //Create
void DrawBullet();  //Update

//-------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------
int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Galaga'88");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	////// Load textures from the resources directory
    //LOGO
    Texture GalagaTitleLogo = LoadTexture("GalagaTitleLogo.png");
    GameScreen currentScreen = LOGO;

    //TITLE
    Texture Galaga88Logo = LoadTexture("GALAGA88_LOGO.png");

    //Gameplay
    Texture player_body = LoadTexture("PlayerGalaga88.png");
    Texture player_bullet = LoadTexture("PlayerBullet.png");

    //Frame
    unsigned int framesCounter = 0;
    SetTargetFPS(60); // Set Game to run 60 Frames per second

    //Particles
    createParticles();

    //-------------------------------------------------------------------------------------
    // Local Variables
    //-------------------------------------------------------------------------------------
    //Player
    player.radius = 25;
    player.position = (Vector2){ GetScreenWidth()/2, GetScreenHeight()*9/10 };



    //-------------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------------
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

        switch (currentScreen)
        {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!

            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > 120)
            {
                UnloadTexture(GalagaTitleLogo);
                currentScreen = TITLE;
            }
        } break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!

            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER)) //  || IsGestureDetected(GESTURE_TAP)
            {
                currentScreen = GAMEPLAY;
            }
        } break;
        case GAMEPLAY:
        {
            //Player Movement
            int player_speed = 9;                                                                                       //La he reducido de 12 a 9, pero tal vez un poco más (Jan)
            if (IsKeyDown(KEY_LEFT)) player.position.x -= player_speed;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += player_speed;

            //Player Collisions
            if ((player.position.x + player.radius) >= GetScreenWidth()) //Right Side
            {
                player.position.x = GetScreenWidth() - player.radius;
            }
            else if ((player.position.x - player.radius) <= 0) //Left Side
            {
                player.position.x = 0 + player.radius;
            }

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                //Crear Instancia de bala
                ShootBullet();
            }




        } break;
        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                //currentScreen = TITLE;
            }
        } break;
        default: break;
        }

        //DRAWING





		BeginDrawing();



        switch (currentScreen)
        {
        case LOGO:
        {
            ClearBackground(BLACK);
            DrawText("Jan Corredor, Alexandre Garcia, Arnau Gonzalez", 75, 750, 45, WHITE);
            DrawTexture(GalagaTitleLogo, 0, 0, WHITE);
        } break;
        case TITLE:
        {
            ClearBackground(BLACK);

            //Particulas
            drawParticles();

            //Logo
            DrawTexture(Galaga88Logo, GetScreenWidth() / 3, GetScreenHeight()/ 10, WHITE);

            //Scores
            DrawText("SCORE", GetScreenWidth() / 20, GetScreenHeight() / 50, 45, WHITE);
                //Insertar Puntuacion
            DrawText("H I SCORE", GetScreenWidth() *7/ 10, GetScreenHeight() / 50, 45, WHITE);
                //Insertar Puntuacion Mas alta

            //Other
            DrawText("PUSH ENTER", GetScreenWidth()/3, GetScreenHeight() /2, 45, GREEN);





        } break;
        case GAMEPLAY:
        {
            ClearBackground(BLACK);

            //Particulas
            drawParticles();

            //Scores
            DrawText("SCORE", GetScreenWidth() / 20, GetScreenHeight() / 50, 45, WHITE);
                //Insertar Puntuacion

            //Player
            //DrawCircle(player.position.x, player.position.y, player.radius, BLUE);                        // Dejamos el circulo o alguna figura geometrica para las colisiones? (Jan)
            DrawTexture(player_body, player.position.x-74, player.position.y-63, WHITE);


            //Bullets
            DrawBullet();

            for (int i = 0; i < player_bullet_counter; i++) //Draw
            {
                DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
            }
            


        } break;
        case ENDING:
        {


        } break;
        default: break;
        }


		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(GalagaTitleLogo);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
//-------------------------------------------------------------------------------------
// Funciones 
//-------------------------------------------------------------------------------------

void createParticles()
{
    Color particle_colors[6] = { GREEN, SKYBLUE, BLUE, RED, WHITE, LIGHTGRAY }; //Particles Possible Colors

    for (int i = 0; i < MAXPARTICLES; i++) //Create
    {
        particles[i].particle_position = (Vector2){ GetRandomValue(GetScreenWidth()/200, GetScreenWidth()*199/200), GetRandomValue(GetScreenHeight() / 200, GetScreenHeight() * 199 / 200) }; //Random position in screen
        particles[i].particle_color = particle_colors[GetRandomValue(0, 5)]; // Pick Random Color
        particles[i].particle_size = 5; // Constant Value
        particles[i].particle_speed = GetRandomValue(1, 3);
        particles[i].visible = true; //Visible by Default
    }
}

void drawParticles()
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
            particles[i].particle_position = (Vector2){ GetRandomValue(GetScreenWidth() / 200, GetScreenWidth() * 199 / 200), GetRandomValue(0, 5) };// Position
            particles[i].particle_color = particle_colors[GetRandomValue(0, 5)];                                                                     //Color
            particles[i].particle_speed = GetRandomValue(1, 3);                                                                                      //Speed
        }

    }

    for (int i = 0; i < MAXPARTICLES; i++) //Draw
    {
        if (particles[i].visible == true) //Draw only if visible
        {
            DrawRectangle(particles[i].particle_position.x, particles[i].particle_position.y, particles[i].particle_size, particles[i].particle_size, particles[i].particle_color);
        }
    }
}

void ShootBullet()
{
    player_bullet_counter += 1;

    playerbullets[player_bullet_counter].bullet_position = (Vector2){ player.position.x, player.position.y};
    playerbullets[player_bullet_counter].bullet_radius = 1;
    playerbullets[player_bullet_counter].bullet_color = BLUE;
}


void DrawBullet()
{
    for (int i = 0; i < player_bullet_counter; i++) //Update
    {
        playerbullets[i].bullet_position.y -= 1;
    }

    for (int i = 0; i < player_bullet_counter; i++) //Draw
    {
        DrawCircle(playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, playerbullets[i].bullet_radius, playerbullets[i].bullet_color);
    }
}