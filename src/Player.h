#pragma once
#include "SpriteSheetLoader.h"
#include <raylib.h>
#include <string>
#include <vector>

class Player
{
public:
    Player();
    Player(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData);
    void Update(float deltaTime);
    void Draw();
    Vector2 GetDrawPosition();
    Rectangle GetCollisionBounds();
    Rectangle GetPreviousPosition();
    void Dispose();

public:
    Rectangle bounds;
    Texture2D spriteSheet;
    Vector2 velocity;
    int score;

private:
    Rectangle currentAnimationBounds;
    Rectangle idleAnimationRegion;
    Rectangle idleAnimationBounds;
    Rectangle runningAnimationRegion;
    Rectangle runningAnimationBounds;
    enum AnimationState {STANDING, JUMPING, RUNNING};
    AnimationState actualState;
    AnimationState previousState;
    int speed;
    int framesCounter;
    int framesSpeed;
    int currentFrame;
};