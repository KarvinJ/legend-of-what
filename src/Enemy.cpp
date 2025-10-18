#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData)
{
    this->spriteSheet = spriteSheet;
    isDead = false;
    isDestroyed = false;
    deadTimer = 0;

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
        (float)runningAnimationRegion.width / 6,
        (float)runningAnimationRegion.height};

    hitAnimationRegion = spriteSheetData["hit"];

    hitAnimationBounds = {
        hitAnimationRegion.x,
        hitAnimationRegion.y,
        (float)hitAnimationRegion.width / 4,
        (float)hitAnimationRegion.height};

    speed = 50;
    velocity = {0, 0};
    framesCounter = 0;
    framesSpeed = 6;
    currentFrame = 0;
}

void Enemy::Update(float deltaTime)
{
}

void Enemy::Draw(float deltaTime)
{
    framesCounter++;

    // HandleAnimationByBounds(hitAnimationBounds, 0, 4, currentFrame, framesCounter, framesSpeed);
    // HandleAnimationByBounds(runningAnimationBounds, 0, 4, currentFrame, framesCounter, 12);

    if (isDead)
    {
        deadTimer += deltaTime;

        HandleAnimationByBounds(hitAnimationBounds, 0, 4, currentFrame, framesCounter, 8);
        DrawTextureRec(spriteSheet, hitAnimationBounds, GetDrawPosition(), WHITE);

        if (deadTimer >= 1)
        {
            isDestroyed = true;
        }
    }
    else
    {
        HandleAnimationByBounds(idleAnimationBounds, 0, 4, currentFrame, framesCounter, framesSpeed);
        DrawTextureRec(spriteSheet, idleAnimationBounds, GetDrawPosition(), WHITE);
    }

    // DrawRectangleRec(GetCollisionBounds(), RED);
}

Vector2 Enemy::GetDrawPosition()
{
    return {bounds.x - bounds.width / 4, bounds.y - bounds.height / 4};
}

Rectangle Enemy::GetCollisionBounds()
{
    return {
        bounds.x,
        bounds.y,
        bounds.width / 2,
        bounds.height / 2};
}

Rectangle Enemy::GetPreviousPosition()
{
    Rectangle collisionBounds = GetCollisionBounds();

    float positionX = collisionBounds.x - velocity.x;
    float positionY = collisionBounds.y - velocity.y;

    return {positionX, positionY, collisionBounds.width, collisionBounds.height};
}

bool Enemy::HasBeenHit(Rectangle hitBounds)
{
    if (CheckCollisionRecs(GetCollisionBounds(), hitBounds))
    {
        isDead = true;
        return true;
    }

    return false;
}

void Enemy::HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame, int &frameCounter, int frameSpeed)
{
    if (framesCounter >= (60 / frameSpeed))
    {
        framesCounter = 0;

        currentFrame++;

        if (currentFrame >= totalFrames)
        {
            if (isDead)
            {
                currentFrame = totalFrames - 1;
            }
            else
            {
                currentFrame = 0;
            }
        }

        animationBounds.x = initialXposition + ((float)currentFrame * (float)animationBounds.width);
    }
}

void Enemy::Dispose()
{
    UnloadTexture(spriteSheet);
}