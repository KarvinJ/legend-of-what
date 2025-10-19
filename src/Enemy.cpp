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
    isMovingRight = true;

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

    speed = 10;
    velocity = {0, 0};
    framesCounter = 0;
    currentFrame = 0;
}

void Enemy::Update(float deltaTime)
{
    if (isDead)
    {
        return;
    }

    if (isMovingRight)
    {
        velocity.x += speed * deltaTime;
    }
    else
    {
        velocity.x -= speed * deltaTime;
    }

    velocity.x *= 0.9f;
    bounds.x += velocity.x;
}

void Enemy::Draw(float deltaTime)
{
    Rectangle currentAnimationBounds = GetCurrentAnimationBounds(deltaTime);

    DrawTextureRec(spriteSheet, currentAnimationBounds, GetDrawPosition(), WHITE);

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

void Enemy::HasCollideWithObstacle(Rectangle obstacleBounds)
{
    if (CheckCollisionRecs(GetCollisionBounds(), obstacleBounds))
    {
        isMovingRight = !isMovingRight;
    }
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

Rectangle Enemy::GetCurrentAnimationBounds(float deltaTime)
{
    Rectangle currentAnimationBounds;

    framesCounter++;

    if (isDead)
    {
        deadTimer += deltaTime;

        HandleAnimationByBounds(hitAnimationBounds, 0, 4, currentFrame, framesCounter, 8);
        currentAnimationBounds = hitAnimationBounds;

        if (deadTimer >= 1)
        {
            isDestroyed = true;
        }
    }
    else
    {
        HandleAnimationByBounds(runningAnimationBounds, 0, 4, currentFrame, framesCounter, 12);
        currentAnimationBounds = runningAnimationBounds;
    }

    if (!isMovingRight)
    {
        currentAnimationBounds.width = currentAnimationBounds.width;
    }

    else
    {
        currentAnimationBounds.width = -currentAnimationBounds.width;
    }

    return currentAnimationBounds;
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