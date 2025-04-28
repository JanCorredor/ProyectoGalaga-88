#pragma once
#include "raylib.h"
#include "resource_dir.h"

class ResourceManager
{
public:
    Texture GrupoDeNombreLogo;
    Texture Galaga88Logo;
    Texture stageindicator1;

    Texture Goei_0;
    Texture Goei_1;

    Texture Zako;
    Texture Bon;

    Texture player_body;

    Texture player_bullet;
    Texture enemybullet_0;
    Texture enemybullet_1;

	ResourceManager()
	{
        //SearchAndSetResourceDir("resources");

        ////// Load textures from the resources directory
        //LOGO
        GrupoDeNombreLogo = LoadTexture("Wiki/Sprites/GrupoDeNombre.png");

        //TITLE
        Galaga88Logo = LoadTexture("HUD/GALAGA88_LOGO.png");

        //Gameplay
        ////HUD
        stageindicator1 = LoadTexture("HUD/Galaga_'88_icon_stage_1.png");

        ////Enemies
        Goei_0 = LoadTexture("Enemies/Goei_0.png");
        Goei_1 = LoadTexture("Enemies/Goei_1.png");

        Zako = LoadTexture("Enemies/Zako.png");
        Bon = LoadTexture("Enemies/Bon.png");

        ////Player
        player_body = LoadTexture("Player/PlayerGalaga88.png");

        ////Bullets
        player_bullet = LoadTexture("Player/PlayerBullet.png");
        enemybullet_0 = LoadTexture("Enemies/BalaEnemigo_0.png");
        enemybullet_1 = LoadTexture("Enemies/BalaEnemigo_1.png");
	}
    ~ResourceManager()
    {

    }
};
