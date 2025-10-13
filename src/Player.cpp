#include "Player.h"

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

    currentAnimationBounds = idleAnimationBounds;

    previousState = STANDING;
    actualState = STANDING;

    speed = 50;
    velocity = {0, 0};
    score = 0;
    framesCounter = 0;
    framesSpeed = 6;
    currentFrame = 0;
}

void Player::Update(float deltaTime)
{
    framesCounter++;

    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
        {
            currentFrame++;

            if (currentFrame > 8)
            {
                currentFrame = 0;
            }

            currentAnimationBounds.x = runningAnimationRegion.x + ((float)currentFrame * (float)currentAnimationBounds.width);
        }
        else
        {
            currentFrame++;

            if (currentFrame > 3)
            {
                currentFrame = 0;
            }

            currentAnimationBounds.x = idleAnimationRegion.x + ((float)currentFrame * (float)currentAnimationBounds.width);
        }
    }

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
    // need to work in my animation system.
    Rectangle tempBounds = currentAnimationBounds;
    if (IsKeyDown(KEY_D))
    {
        currentAnimationBounds = runningAnimationBounds;
        tempBounds.width = currentAnimationBounds.width;
    }
    else if (IsKeyDown(KEY_A))
    {
        currentAnimationBounds = runningAnimationBounds;
        tempBounds.width = -currentAnimationBounds.width;
    }
    else
    {
        currentAnimationBounds = idleAnimationBounds;
    }

    Vector2 drawPosition = GetDrawPosition();
    DrawTextureRec(spriteSheet, tempBounds, drawPosition, WHITE);

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

    float positionX = bounds.x - velocity.x;
    float positionY = bounds.y - velocity.y;

    return {positionX, positionY, collisionBounds.width, collisionBounds.height};
}

void Player::Dispose()
{
    UnloadTexture(spriteSheet);
}
