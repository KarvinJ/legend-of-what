#pragma once
#include "SpriteSheetLoader.h"
#include <raylib.h>
#include <string>
#include <vector>

class Player
{
public:
    Player();
    Player(float positionX, float positionY, Texture2D &sprite, vector<TextureInfo> textureInfos);
    void Update(float deltaTime);
    void Draw();
    Vector2 GetDrawPosition();
    Rectangle GetCollisionBounds();
    Rectangle GetPreviousPosition();
    void Dispose();

public:
    Rectangle bounds;
    Texture2D sprite;
    Vector2 velocity;
    int score;

private:
    Rectangle animationBounds;
    TextureInfo textureInfo;
    int speed;
    int framesCounter;
    int framesSpeed;
    int currentFrame;
};