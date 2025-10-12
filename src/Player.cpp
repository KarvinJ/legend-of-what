#include "Player.h"

Player::Player()
{
}

Player::Player(float positionX, float positionY, const std::string &spritePath)
{
    sprite = LoadTexture(spritePath.c_str());
    bounds = {positionX, positionY, (float)sprite.width / 4, (float)sprite.height};
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
    Rectangle collisionBounds = GetCollisionBounds();

    DrawTextureRec(sprite, {0, 0, 64, 80}, {bounds.x, bounds.y - bounds.height / 2 / 2}, WHITE);
    DrawRectangleRec(collisionBounds, WHITE);
}

Rectangle Player::GetCollisionBounds()
{
    return {
        bounds.x + bounds.width / 2 / 2,
        bounds.y,
        bounds.width / 2,
        bounds.height / 2};
}

Rectangle Player::GetPreviousPosition()
{
    Rectangle collisionBounds = GetCollisionBounds();

    float positionX = collisionBounds.x - velocity.x;
    float positionY = collisionBounds.y - velocity.y;

    return {positionX, positionY, collisionBounds.width, collisionBounds.height};
}

void Player::Dispose()
{
    UnloadTexture(sprite);
}
