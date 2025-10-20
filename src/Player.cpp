#include "Player.h"
#include <iostream>

Player::Player()
{
}

Player::Player(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData)
{
    this->spriteSheet = spriteSheet;
    isDead = false;
    isMovingRight = true;
    deadTimer = 0;

    spawnPosition = {positionX, positionY};
    idleAnimationRegion = spriteSheetData["idle"];
    bounds = {positionX, positionY, (float)idleAnimationRegion.width / 4, (float)idleAnimationRegion.height};
    attackBounds = {positionX, positionY, 32, 32};

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

    jumpingAnimationRegion = spriteSheetData["jump"];

    jumpingAnimationBounds = {
        jumpingAnimationRegion.x,
        jumpingAnimationRegion.y,
        (float)jumpingAnimationRegion.width / 15,
        (float)jumpingAnimationRegion.height};

    dyingAnimationRegion = spriteSheetData["dead"];

    dyingAnimationBounds = {
        dyingAnimationRegion.x,
        dyingAnimationRegion.y,
        (float)dyingAnimationRegion.width / 8,
        (float)dyingAnimationRegion.height};

    attackingAnimationRegion = spriteSheetData["attack"];

    attackingAnimationBounds = {
        attackingAnimationRegion.x,
        attackingAnimationRegion.y,
        (float)attackingAnimationRegion.width / 8,
        (float)attackingAnimationRegion.height};

    attackAnimationTimer = 0;
    attackAnimationFrame = 0;

    previousState = Player::STANDING;
    actualState = Player::STANDING;

    speed = 40;
    velocity = {0, 0};
    framesCounter = 0;
    framesSpeed = 6;
    currentFrame = 0;
}

void Player::HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame, int &frameCounter, int frameSpeed)
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

void Player::Update(float deltaTime)
{
    velocity.y += 20.8f * deltaTime;

    bounds.y += velocity.y;
    bounds.x += velocity.x;

    if (!isDead)
    {
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
            bounds.x = spawnPosition.x;
            bounds.y = spawnPosition.y;
            velocity.y = 0;
        }
    }
    else
    {
        deadTimer += deltaTime;

        if (deadTimer >= 2)
        {
            isDead = false;
            deadTimer = 0;
            actualState = Player::STANDING;
            bounds.x = spawnPosition.x;
            bounds.y = spawnPosition.y;
        }
    }
}

void Player::Draw(float deltaTime)
{
    Rectangle currentAnimationBounds = GetCurrentAnimationBounds(deltaTime);

    DrawTextureRec(spriteSheet, currentAnimationBounds, GetDrawPosition(), WHITE);

    Rectangle collisionBounds = GetCollisionBounds();

    if (isMovingRight)
    {
        attackBounds.x = collisionBounds.x + collisionBounds.width;
    }
    else
    {
        attackBounds.x = collisionBounds.x - collisionBounds.width / 4;
    }

    attackBounds.y = collisionBounds.y;

    // DrawRectangleRec(attackBounds, GREEN);
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

bool Player::HasBeenHit(Rectangle enemyBounds)
{
    if (CheckCollisionRecs(GetCollisionBounds(), enemyBounds))
    {
        isDead = true;
        return true;
    }

    return false;
}

void Player::Dispose()
{
    UnloadTexture(spriteSheet);
}

Player::AnimationState Player::GetCurrentAnimationState()
{
    if (isDead)
        return Player::DYING;

    else if (velocity.y < 0 || (velocity.y > 0 && previousState == Player::JUMPING))
        return Player::JUMPING;

    else if (IsKeyDown(KEY_E) && attackAnimationTimer < 0.3)
        return Player::ATTACKING;

    else if (velocity.y > 0)
        return Player::FALLING;

    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
        return Player::RUNNING;

    return Player::STANDING;
}

Rectangle Player::GetCurrentAnimationBounds(float deltaTime)
{
    actualState = GetCurrentAnimationState();

    Rectangle currentAnimationBounds;

    framesCounter++;

    if (IsKeyDown(KEY_E))
    {
        attackAnimationTimer += deltaTime;
    }
    else
    {
        attackAnimationTimer = 0;
        attackAnimationFrame = 0;
    }

    switch (actualState)
    {

    case DYING:
        HandleAnimationByBounds(dyingAnimationBounds, dyingAnimationRegion.x, 8, currentFrame, framesCounter, framesSpeed);
        currentAnimationBounds = dyingAnimationBounds;
        break;

    case ATTACKING:

        if (attackAnimationTimer < 0.3)
        {
            HandleAnimationByBounds(attackingAnimationBounds, attackingAnimationRegion.x, 4, attackAnimationFrame, framesCounter, 12);
            currentAnimationBounds = attackingAnimationBounds;
        }
        else
        {
            HandleAnimationByBounds(idleAnimationBounds, idleAnimationRegion.x, 4, currentFrame, framesCounter, framesSpeed);
            currentAnimationBounds = idleAnimationBounds;
        }

        break;

    case RUNNING:
        HandleAnimationByBounds(runningAnimationBounds, runningAnimationRegion.x, 8, currentFrame, framesCounter, 12);
        currentAnimationBounds = runningAnimationBounds;
        break;

    case JUMPING:
        HandleAnimationByBounds(jumpingAnimationBounds, jumpingAnimationRegion.x, 15, currentFrame, framesCounter, framesSpeed);
        currentAnimationBounds = jumpingAnimationBounds;
        break;

    case FALLING:
    case STANDING:
    default:
        HandleAnimationByBounds(idleAnimationBounds, idleAnimationRegion.x, 4, currentFrame, framesCounter, framesSpeed);
        currentAnimationBounds = idleAnimationBounds;
    }

    if (velocity.x < 0)
    {
        isMovingRight = false;
    }
    else if (velocity.x > 0)
    {
        isMovingRight = true;
    }

    if (isMovingRight)
    {
        currentAnimationBounds.width = currentAnimationBounds.width;
    }

    else
    {
        currentAnimationBounds.width = -currentAnimationBounds.width;
    }

    previousState = actualState;

    return currentAnimationBounds;
}