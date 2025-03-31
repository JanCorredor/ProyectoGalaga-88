//#include "Game.h"
//#include "Player.h"
//#include "Particles.h"
//
//static Player player = Player::Player();
//
//Game::Game() 
//{
//
//}
//
//Game::~Game()
//{
//
//}
//
//void Game::Initialise() 
//{
//	this->hardmode = false;
//
//    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
//
//    InitWindow(800, 1280, "Galaga'88"); // 1280, 800
//
//    SearchAndSetResourceDir("resources");
//
//    ////// Load textures from the resources directory
//    //LOGO
//    GalagaTitleLogo = LoadTexture("HUD/GalagaTitleLogo.png");
//    currentScreen = LOGO;
//
//    //TITLE
//    Galaga88Logo = LoadTexture("HUD/GALAGA88_LOGO.png");
//
//    //Gameplay
//    ////HUD
//    stageindicator1 = LoadTexture("HUD/Galaga_'88_icon_stage_1.png");
//
//
//    ////Player
//    player_body = LoadTexture("Player/PlayerGalaga88.png");
//
//    ////Enemies
//    enemy1_0 = LoadTexture("Enemies/Enemy1_0.png");
//    enemy1_1 = LoadTexture("Enemies/Enemy1_1.png");
//
//    ////Bullets
//    player_bullet = LoadTexture("Player/PlayerBullet.png");
//    enemybullet_0 = LoadTexture("Enemies/BalaEnemigo_0.png");
//    enemybullet_1 = LoadTexture("Enemies/BalaEnemigo_1.png");
//
//    //Audio
//    InitAudioDevice();      // Initialize audio device
//
//    ////Ost and Sounds
//    buttonclick = LoadSound("Sounds/Click.wav");
//    GalagaOpening = LoadSound("Sounds/GalagaOpening.wav");
//    GalagaWin = LoadSound("Sounds/GalagaWin.wav");
//    GalagaDefeat = LoadSound("Sounds/GalagaDefeat.wav");
//
//    ////Player
//    playerShoot = LoadSound("Sounds/PlayerShoot.wav");
//    playerDeath = LoadSound("Sounds/PlayerDeath.wav");
//
//    ////Enemy
//    enemyShoot = LoadSound("Sounds/EnemyShoot.wav");
//    enemyFormation = LoadSound("Sounds/EnemyFormation.wav");
//    enemyDeathExplosion = LoadSound("Sounds/EnemyDeathExplosion.wav");
//
//    //Frame
//    framesCounter = 0;
//    SetTargetFPS(60); // Set Game to run 60 Frames per second
//
//    //Particles
//    createParticles();
//
//    //-------------------------------------------------------------------------------------
//    // Local Variables
//    //-------------------------------------------------------------------------------------
//    //Player
//    player.SetPosition({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() * 9 / 10 });
//}
//
//void Game::Update() 
//{
//    //-------------------------------------------------------------------------------------
//   // Game Loop
//   //-------------------------------------------------------------------------------------
//    while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
//    {
//        switch (currentScreen)
//        {
//        case LOGO:
//        {
//            framesCounter++;    // Count frames
//
//            // Wait for 2 seconds (120 frames) before jumping to TITLE 
//            if (framesCounter > 120)
//            {
//                UnloadTexture(GalagaTitleLogo);
//                currentScreen = TITLE;
//            }
//        } break;
//        case TITLE:
//        {
//            PlaySound(GalagaOpening); /////////////////////////////////////////////////No funciona al momento
//            if (IsKeyPressed(KEY_UP))
//            {
//                hardmode = true;
//            }
//            if (IsKeyPressed(KEY_ENTER)) //  || IsGestureDetected(GESTURE_TAP)
//            {
//                StopSound(GalagaOpening); //////////////////////////////////////////Si comentas esto se escucha en Gameplay
//                PlaySound(buttonclick);
//                currentScreen = GAMEPLAY;
//            }
//        } break;
//        case GAMEPLAY:
//        {
//            if (enemybullets.empty() == true)
//            {
//                createEnemies();
//            }
//
//            framesCounter++;
//
//            //Player
//            player.Move();
//
//            //Shoot
//            if (IsKeyPressed(KEY_SPACE))
//            {
//                PlaySound(playerShoot);
//                player_bullet_counter += 1;
//                ShootBullet(); //Crear Instancia de bala
//            }
//
//
//            moveEnemiesCircle();
//
//            //Enemy Collisions
//            for (int i = 0; i < MAXENEMIES; i++)
//            {
//                for (int j = 0; j < playerbullets.size(); j++)
//                {
//                    if (enemies[i].isEnemyAlive() == true) {
//                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, enemies[i].getEnemyPosition(), enemies[i].getEnemyRadius()))
//                        {
//                            player.SumScore(100);
//                            hit_counter++;
//                            PlaySound(enemyDeathExplosion);
//                            enemies[i].setEnemyLife(false);
//                            playerbullets.erase(playerbullets.begin() + j);
//                        }
//                    }
//                }
//            }
//
//            if (player.GetLives() < 0)
//            {
//                currentScreen = ENDING;
//            }
//
//        } break;
//        case ENDING:
//        {
//            int score = player.GetScore();
//
//            if (player.GetLives() > 0)
//            {
//                PlaySound(GalagaWin);
//            }
//            else
//            {
//                PlaySound(GalagaDefeat);
//            }
//
//            player = Player::Player();
//            player.SetScore(score);
//
//            enemybullets.clear();
//            playerbullets.clear();
//
//            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
//            {
//                StopSound(GalagaWin);
//                StopSound(GalagaDefeat);
//                player_bullet_counter = 0;
//                hit_counter = 0;
//                currentScreen = TITLE;
//            }
//        } break;
//        default: break;
//        }
//}
//
//void Game::Render() 
//{
//    //-------------------------------------------------------------------------------------
////DRAWING
////-------------------------------------------------------------------------------------
//    BeginDrawing();
//
//
//    switch (this->state)
//    {
//    case LOGO:
//    {
//        ClearBackground(BLACK);
//        DrawText("Jan Corredor", GetScreenWidth() / 3, GetScreenHeight() * 5 / 10, 45, WHITE);
//        DrawText("Arnau Gonzalez", GetScreenWidth() / 3, GetScreenHeight() * 6 / 10, 45, WHITE);
//        DrawText("Alexandre Garcia", GetScreenWidth() / 3, GetScreenHeight() * 7 / 10, 45, WHITE);
//        DrawTextureEx(GalagaTitleLogo, { 0, 0 }, 0, 0.65, WHITE);
//    } break;
//    case TITLE:
//    {
//        ClearBackground(BLACK);
//
//        //Particulas
//        drawParticles();
//
//        //Logo
//        DrawTexture(Galaga88Logo, GetScreenWidth() / 4, GetScreenHeight() / 10, WHITE);
//
//        //Scores
//        DrawText("SCORE", GetScreenWidth() / 20, GetScreenHeight() / 50, 45, WHITE);
//        DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
//
//        DrawText("H I SCORE", GetScreenWidth() * 7 / 10, GetScreenHeight() / 50, 45, WHITE);
//        //Insertar Puntuacion Mas alta
//
//    //Other
//        DrawText("PUSH ENTER", GetScreenWidth() / 3, GetScreenHeight() / 2, 45, GREEN);
//    } break;
//    case GAMEPLAY:
//    {
//        ClearBackground(BLACK);
//
//        //Particulas
//        drawParticles();
//
//
//        //UI
//        ////Scores
//        DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
//        DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);
//
//        DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
//        DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);
//
//        //// Lives Remaining
//        for (int i = 0; i < player.GetLives(); i++)
//        {
//            DrawTexture(player_body, 74 * i - GetScreenWidth() / 30, GetScreenHeight() * 9 / 10, WHITE);
//        }
//
//        ////Stage Indicator
//        DrawTexture(stageindicator1, GetScreenWidth() * 95 / 100, GetScreenHeight() * 92 / 100, WHITE);
//
//        //Bullets
//        DrawBullet();
//
//        for (int i = 0; i < playerbullets.size(); i++) //Esto deberia estar en DrawBullets
//        {
//            DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
//        }
//
//        if (hardmode == 0)
//        {
//            DrawEnemyBullet();
//        }
//        else
//        {
//            DrawGodShot();
//        }
//
//
//        for (int i = 0; i < enemybullets.size(); i++) //Esto deberia estar en DrawBullets
//        {
//            int x = GetRandomValue(0, 1);
//            if (x == 0)
//            {
//                DrawTexture(enemybullet_0, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
//            }
//            else
//            {
//                DrawTexture(enemybullet_1, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
//            }
//        }
//
//        //Player
//        Vector2 playerActualPosition = player.GetPosition();
//        if (player.GetInmortal() == false) {
//            DrawTexture(player_body, playerActualPosition.x - 74, playerActualPosition.y - 63, WHITE);
//        }
//
//
//
//        //Enemies
//
//        for (int i = 0; i < MAXENEMIES; i++)
//        {
//            if (enemies[i].isEnemyAlive() == true)
//            {
//                Vector2 Correccion = { enemies[i].getEnemyPosition().x - 33, enemies[i].getEnemyPosition().y - 37 }; //-70 -74
//                DrawTextureEx(enemy1_0, Correccion, 0, 0.5, WHITE);
//            }
//        }
//
//    } break;
//    case ENDING:
//    {
//        ClearBackground(BLACK);
//        DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
//        DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);
//
//        DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
//        DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);
//
//        DrawText("SHOTS FIRED", GetScreenWidth() / 13, GetScreenHeight() / 10, 45, YELLOW);
//        DrawText(TextFormat("%i", (char*)player_bullet_counter), GetScreenWidth() * 2 / 3, GetScreenHeight() / 10, 45, YELLOW);
//
//        DrawText("NUMBER OF HITS", GetScreenWidth() / 13, GetScreenHeight() / 7, 45, YELLOW);
//        DrawText(TextFormat("%i", (char*)hit_counter), GetScreenWidth() * 2 / 3, GetScreenHeight() / 7, 45, YELLOW);
//
//        float ratio;
//        if (player_bullet_counter == 0)
//        {
//            ratio = 0.00f;
//        }
//        else
//        {
//            ratio = ((float)hit_counter / (float)player_bullet_counter) * 100;
//        }
//
//        DrawText("HIT-MISS RATIO", GetScreenWidth() / 13, GetScreenHeight() / 5, 45, YELLOW);
//        DrawText(TextFormat("%d %%", (char)ratio), GetScreenWidth() * 2 / 3, GetScreenHeight() / 5, 45, YELLOW); ///////////////Error: No se dibujan los decimales
//
//        DrawText("You DIED", GetScreenWidth() / 3, GetScreenHeight() * 5 / 10, 45, RED);
//        DrawText("Skill Issue", GetScreenWidth() / 3, GetScreenHeight() * 6 / 10, 45, WHITE);
//        DrawText("Retry?", GetScreenWidth() / 3, GetScreenHeight() * 7 / 10, 45, GREEN);
//
//    } break;
//    default: break;
//    }
//
//
//    EndDrawing();
//}
//
//void Game::Cleanup()
//{
//	
//}