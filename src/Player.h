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
    void Draw(float deltaTime);
    Vector2 GetDrawPosition();
    Rectangle GetCollisionBounds();
    Rectangle GetPreviousPosition();
    void Dispose();

public:
    Rectangle bounds;
    Rectangle attackBounds;
    Vector2 velocity;
    enum AnimationState
    {
        STANDING,
        RUNNING,
        JUMPING,
        FALLING,
        ATTACKING,
        DYING,
    };

private:
    bool isDead;
    float deadTimer;
    Texture2D spriteSheet;
    Rectangle idleAnimationRegion;
    Rectangle idleAnimationBounds;
    Rectangle runningAnimationRegion;
    Rectangle runningAnimationBounds;
    Rectangle jumpingAnimationRegion;
    Rectangle jumpingAnimationBounds;
    Rectangle dyingAnimationRegion;
    Rectangle dyingAnimationBounds;
    Rectangle attackingAnimationRegion;
    Rectangle attackingAnimationBounds;
    float attackAnimationTimer;
    int attackAnimationFrame;
    AnimationState GetCurrentAnimationState();
    Rectangle GetCurrentAnimationBounds(float deltaTime);
    void HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame, int &frameCounter, int frameSpeed);
    AnimationState actualState;
    AnimationState previousState;
    int speed;
    int framesCounter;
    int framesSpeed;
    int currentFrame;
};