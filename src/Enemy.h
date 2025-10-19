#pragma once
#include "SpriteSheetLoader.h"
#include <raylib.h>
#include <string>
#include <vector>

class Enemy
{
public:
    Enemy();
    Enemy(float positionX, float positionY, Texture2D &spriteSheet, unordered_map<string, Rectangle> &spriteSheetData);
    void Update(float deltaTime);
    void Draw(float deltaTime);
    Vector2 GetDrawPosition();
    Rectangle GetCollisionBounds();
    Rectangle GetPreviousPosition();
    void HasCollideWithObstacle(Rectangle obstacleBounds);
    bool HasBeenHit(Rectangle hitBounds);
    void Dispose();

public:
    Rectangle bounds;
    Vector2 velocity;
    bool isDead;
    bool isDestroyed;

private:
    bool isMovingRight;
    float deadTimer;
    Texture2D spriteSheet;
    Rectangle idleAnimationRegion;
    Rectangle idleAnimationBounds;
    Rectangle runningAnimationRegion;
    Rectangle runningAnimationBounds;
    Rectangle hitAnimationRegion;
    Rectangle hitAnimationBounds;
    Rectangle GetCurrentAnimationBounds(float deltaTime);
    void HandleAnimationByBounds(Rectangle &animationBounds, float initialXposition, int totalFrames, int &currentFrame, int &frameCounter, int frameSpeed);
    int speed;
    int framesCounter;
    int currentFrame;
};