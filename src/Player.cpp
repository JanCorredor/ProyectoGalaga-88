#include "Player.h"

//------------------------- Defines -------------------------
typedef struct Bullet {
    Vector2 bullet_position;
    int bullet_radius;
    Color bullet_color;
} Bullet;


//------------------------- Constructores -------------------------

Player::Player()
{
	this->position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() * 9 / 10 };
}

Player::Player(Vector2 _position, int _radius, Color _color, int _score, int _lives)
{
    this->position = _position;
    this->radius = _radius;
    this->color = _color;
    this->score = _score;
    this->lives = _lives;
};

//------------------------- Getters y setters -------------------------

Vector2 Player::GetPosition() { return this->position; }
void Player::SetPosition(Vector2 newVector) { this->position = newVector; }
float Player::GetPositionX() { return this->position.x; }
void Player::SetPositionX(float newPositionX) { this->position.x = newPositionX; }
float Player::GetPositionY() { return this->position.y; }
void Player::SetPositionY(float newPositionY) { this->position.y = newPositionY; }

int Player::GetRadius() { return this->radius; }
void Player::SetRadius(int newRadius) { this->radius = newRadius; }

Color Player::GetColor() { return this->color; }
void Player::SetColor(Color newColor) { this->color = newColor; }

int Player::GetScore() { return this->score; }
void Player::SetScore(int newScore) { this->score = newScore; }

int Player::GetLives() { return this->lives; }
void Player::SetLives(int newLives) { this->lives = newLives; }

//------------------------- Funciones -------------------------

void Player::SumScore(int n)
{
	this->score += n;
}

void Player::Shoot()
{
    Bullet newBullet;
    Vector2 playerActualPosition = this->GetPosition();
    //player_bullet_counter++;

    newBullet.bullet_position = { playerActualPosition.x - 7, playerActualPosition.y - 30 };

    
    if (IsKeyDown(KEY_LEFT))
    {
        newBullet.bullet_position.x -= 9; //Player Speed
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        newBullet.bullet_position.x += 9; //Player Speed
    }

    newBullet.bullet_radius = 10;
    newBullet.bullet_color = BLUE;
    //playerbullets.push_back(newBullet);
}

void Player::Death()
{

}

void Player::Move()
{
    Vector2 playerActualPosition = this->GetPosition();

    //Player Movement
    int player_speed = 9;
    if (IsKeyDown(KEY_LEFT))
    {
        this->SetPositionX(playerActualPosition.x -= player_speed);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        this->SetPositionX(playerActualPosition.x += player_speed);
    }

    //Player Collision With Screen
    if ((playerActualPosition.x + this->GetRadius()) >= GetScreenWidth()) //Right Side
    {
        this->SetPositionX(GetScreenWidth() - this->GetRadius());
    }
    else if ((playerActualPosition.x - this->GetRadius()) <= 0) //Left Side
    {
        this->SetPositionX(this->GetRadius());
    }
}
