#pragma once
#include <raylib.h>
#include <vector>
#include "SpriteSheetLoader.h"
#include "Player.h"
#include "Enemy.h"

class Game
{

public:
    Game();
    ~Game();
    void Update(float deltaTime);
    void Draw(float deltaTime);
    void Run();
    bool CheckCollisionInX(Rectangle bounds, Rectangle platform);
    bool CheckCollisionInY(Rectangle bounds, Rectangle platform);
    void ManageStructureCollision(float deltaTime);

private:
    bool isGamePaused;
    float screenWidth;
    float screenHeight;
    Camera2D camera;
    std::vector<Rectangle> platformBounds;
    Player player;
    Enemy enemy;
    Sound actionSound;
    Music music;
};