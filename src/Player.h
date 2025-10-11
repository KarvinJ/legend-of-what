#pragma once
#include <raylib.h>
#include <string>

class Player
{
public:
    Player();
    Player(float positionX, float positionY, const std::string &spritePath);
    void Update(float deltaTime);
    void Draw();
    void Dispose();
    Rectangle GetPreviousPosition();

public:
    Rectangle bounds;
    Texture2D sprite;
    Vector2 velocity;
    int score;

private:
    int speed;
};