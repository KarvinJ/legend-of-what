#include "Player.h"

Player::Player(float positionX, float positionY, const std::string &spritePath)
{
    sprite = LoadTexture("assets/img/alien.png");
    bounds = {positionX, positionY, (float)sprite.width, (float)sprite.height};
    speed = 600;
    score = 0;
}

void Player::Update(float deltaTime)
{
    if (IsKeyDown(KEY_W))
    {
        bounds.y -= speed * deltaTime;
    }

    else if (IsKeyDown(KEY_S))
    {
        bounds.y += speed * deltaTime;
    }

    else if (IsKeyDown(KEY_D))
    {
        bounds.x += speed * deltaTime;
    }

    else if (IsKeyDown(KEY_A))
    {
        bounds.x -= speed * deltaTime;
    }
}

void Player::Draw()
{
    DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}

void Player::Dispose()
{
    UnloadTexture(sprite);
}
