#include "Player.h"

Player::Player()
{
}

Player::Player(float positionX, float positionY, const std::string &spritePath)
{
    sprite = LoadTexture("assets/img/alien.png");
    bounds = {positionX, positionY, (float)sprite.width, (float)sprite.height};
    speed = 50;
    velocity = {0, 0};
    score = 0;
}

void Player::Update(float deltaTime)
{
    velocity.y += 20.8f * deltaTime;

    bounds.y += velocity.y;
    bounds.x += velocity.x;

    if (IsKeyDown(KEY_D))
    {
        velocity.x += speed * deltaTime;
    }

    else if (IsKeyDown(KEY_A))
    {
        velocity.x -= speed * deltaTime;
    }

    velocity.x *= 0.9f;

    if (bounds.y > 1280)
    {
        bounds.y = 400 - bounds.height;
        bounds.x = 500;
        velocity.y = 0;
    }
}

void Player::Draw()
{
    // DrawTexture(sprite, bounds.x, bounds.y, WHITE);
    DrawRectangleRec(bounds, WHITE);
}

void Player::Dispose()
{
    UnloadTexture(sprite);
}

Rectangle Player::GetPreviousPosition()
{
    float positionX = bounds.x - velocity.x;
    float positionY = bounds.y - velocity.y;

    return {positionX, positionY, bounds.width, bounds.height};
}
