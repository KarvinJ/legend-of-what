#include "Player.h"
#include <iostream>

Player::Player()
{
}

Player::Player(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData)
{
    this->spriteSheet = spriteSheet;
    isDead = false;
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

    previousState = Player::STANDING;
    actualState = Player::STANDING;

    speed = 50;
    velocity = {0, 0};
    framesCounter = 0;
    framesSpeed = 6;
    currentFrame = 0;
}

void Player::HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame, int &frameCounter, int actualFrameSpeed)
{
    if (framesCounter >= (60 / actualFrameSpeed))
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
            bounds.y = 400 - bounds.height;
            bounds.x = 500;
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
        }
    }
}

void Player::Draw()
{
    if (actualState == Player::RUNNING)
    {
        DrawText("Running", 400, 400, 32, WHITE);
    }
    else if (actualState == Player::JUMPING)
    {
        DrawText("Jumping", 400, 400, 32, WHITE);
    }
    else if (actualState == Player::DYING)
    {
        DrawText("Dying", 400, 400, 32, WHITE);
    }
    else if (actualState == Player::ATTACKING)
    {
        DrawText("Attacking", 400, 400, 32, WHITE);
    }
    else
    {
        DrawText("Standing", 400, 400, 32, WHITE);
    }

    framesCounter++;

    if (actualState == Player::RUNNING)
    {
        HandleAnimationByBounds(runningAnimationBounds, runningAnimationRegion.x, 8, currentFrame, framesCounter, 12);
    }
    else if (actualState == Player::JUMPING)
    {
        HandleAnimationByBounds(jumpingAnimationBounds, jumpingAnimationRegion.x, 15, currentFrame, framesCounter, framesSpeed);
    }
    else if (actualState == Player::DYING)
    {
        HandleAnimationByBounds(dyingAnimationBounds, dyingAnimationRegion.x, 8, currentFrame, framesCounter, framesSpeed);
    }
    else
    {
        HandleAnimationByBounds(idleAnimationBounds, idleAnimationRegion.x, 4, currentFrame, framesCounter, framesSpeed);
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
    if (isDead)
        return Player::DYING;

    else if (velocity.y < 0 || (velocity.y > 0 && previousState == Player::JUMPING))
        return Player::JUMPING;

    else if (velocity.y > 0)
        return Player::FALLING;

    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
        return Player::RUNNING;

    return Player::STANDING;
}

Rectangle Player::GetCurrentAnimationBounds()
{
    actualState = GetCurrentAnimationState();

    Rectangle currentAnimationBounds;

    switch (actualState)
    {

    case DYING:
        currentAnimationBounds = dyingAnimationBounds;
        break;

    case RUNNING:
        currentAnimationBounds = runningAnimationBounds;
        break;

    case JUMPING:
        currentAnimationBounds = jumpingAnimationBounds;
        break;

    case FALLING:
    case STANDING:
    default:
        currentAnimationBounds = idleAnimationBounds;
    }

    if (!isDead && IsKeyDown(KEY_D))
    {
        currentAnimationBounds.width = currentAnimationBounds.width;
    }

    else if (!isDead && IsKeyDown(KEY_A))
    {
        currentAnimationBounds.width = -currentAnimationBounds.width;
    }

    previousState = actualState;

    return currentAnimationBounds;
}