#pragma once
#include <raylib.h>
#include <vector>
#include "SpriteSheetLoader.h"
#include "Player.h"

class Game
{

public:
    Game();
    ~Game();
    void Update();
    void Draw();
    bool CheckCollisionInX(Rectangle bounds, Rectangle platform);
    bool CheckCollisionInY(Rectangle bounds, Rectangle platform);
    void ManageStructureCollision(float deltaTime);

private:
    bool isGamePaused;
    float screenWidth;
    float screenHeight;
    Camera2D camera;
    std::vector<Rectangle> collisionBounds;
    Player player;
    Sound actionSound;
    Music music;
};