#include "Player.h"

Player::Player()
{
}

Player::Player(float positionX, float positionY, const std::string &spritePath)
{
    sprite = LoadTexture(spritePath.c_str());

    vector<TextureInfo> textureInfos = loadSpriteSheet("assets/img/characters/character-spritesheet.txt");

    textureInfo = getTextureInfoByName(textureInfos, "idle");

    bounds = {positionX, positionY, (float)textureInfo.bounds.width / 4, (float)textureInfo.bounds.height};

    animationBounds = {textureInfo.bounds.x, textureInfo.bounds.y, (float)textureInfo.bounds.width / 4, (float)textureInfo.bounds.height};
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
        currentFrame++;

        if (currentFrame > 3)
        {
            currentFrame = 0;
        }

        animationBounds.x = textureInfo.bounds.x + ((float)currentFrame * (float)animationBounds.width);
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
    Vector2 drawPosition = GetDrawPosition();

    Rectangle tempBounds = animationBounds;
    if (velocity.x > 0)
    {
        tempBounds.width = animationBounds.width;
    }
    else
    {
        tempBounds.width = -animationBounds.width;
    }

    DrawTextureRec(sprite, tempBounds, drawPosition, WHITE);

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
    UnloadTexture(sprite);
}
