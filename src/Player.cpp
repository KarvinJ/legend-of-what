#include "Player.h"
#include <iostream>

Player::Player()
{
}

Player::Player(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData)
{
    this->spriteSheet = spriteSheet;

    idleAnimationRegion = spriteSheetData["idle"];
    bounds = {positionX, positionY, (float)idleAnimationRegion.width / 4, (float)idleAnimationRegion.height};

    idleAnimationBounds = {
        idleAnimationRegion.x,
        idleAnimationRegion.y,
        (float)idleAnimationRegion.width / 4,
        (float)idleAnimationRegion.height};

    runningAnimationRegion = spriteSheetData["run"];

    runningAnimationBounds = {
        runningAnimationRegion.x,
        runningAnimationRegion.y,
        (float)runningAnimationRegion.width / 8,
        (float)runningAnimationRegion.height};

    previousState = Player::STANDING;
    actualState = Player::STANDING;

    speed = 50;
    velocity = {0, 0};
    framesCounter = 0;
    framesSpeed = 6;
    currentFrame = 0;
}

void Player::HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame)
{
    currentFrame++;

    if (currentFrame > totalFrames)
    {
        currentFrame = 0;
    }

    animationBounds.x = initialXposition + ((float)currentFrame * (float)animationBounds.width);
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
    if (actualState == Player::RUNNING)
    {
        DrawText("Running", 400, 300, 48, WHITE);
    }
    else
    {
        DrawText("Standing", 400, 300, 48, WHITE);
    }

    framesCounter++;

    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;

        if (actualState == Player::RUNNING)
        {
            HandleAnimationByBounds(runningAnimationBounds, runningAnimationRegion.x, 7, currentFrame);
        }
        else
        {
            HandleAnimationByBounds(idleAnimationBounds, idleAnimationRegion.x, 3, currentFrame);
        }
    }

    Rectangle currentAnimationBounds = GetCurrentAnimationBounds();
    Vector2 drawPosition = GetDrawPosition();

    DrawTextureRec(spriteSheet, currentAnimationBounds, drawPosition, WHITE);

    // Rectangle collisionBounds = GetCollisionBounds();
    // DrawRectangleRec(collisionBounds, WHITE);
}

Vector2 Player::GetDrawPosition()
{
    return {bounds.x - bounds.width / 4, bounds.y - bounds.height / 4};
}

Rectangle Player::GetCollisionBounds()
{
    return {
        bounds.x,
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
    UnloadTexture(spriteSheet);
}

Player::AnimationState Player::GetCurrentAnimationState()
{
    bool isPlayerMoving = IsKeyDown(KEY_A) || IsKeyDown(KEY_D);

    if (isPlayerMoving)
        return Player::RUNNING;

    return Player::STANDING;
}

Rectangle Player::GetCurrentAnimationBounds()
{
    actualState = GetCurrentAnimationState();

    Rectangle currentAnimationBounds;

    switch (actualState)
    {

    case RUNNING:
        currentAnimationBounds = runningAnimationBounds;
        break;

    default:
        currentAnimationBounds = idleAnimationBounds;
    }

    if (IsKeyDown(KEY_D))
    {
        currentAnimationBounds.width = currentAnimationBounds.width;
    }

    else if (IsKeyDown(KEY_A))
    {
        currentAnimationBounds.width = -currentAnimationBounds.width;
    }

    previousState = actualState;

    return currentAnimationBounds;
}