#pragma once
#include <raylib.h>
#include <math.h>
#include <vector>
#include "SpriteSheetLoader.h"
#include "Player.h"
#include "Enemy.h"

class Game
{

public:
    Game();
    ~Game();
    void Run();

private:
    void Update(float deltaTime);
    void Draw(float deltaTime);
    bool CheckCollisionInX(Rectangle bounds, Rectangle platform);
    bool CheckCollisionInY(Rectangle bounds, Rectangle platform);
    void ManageStructureCollision(float deltaTime);
    void CameraController(Camera2D &camera);

private:
    bool isGamePaused;
    bool isDebugCamera;
    float screenWidth;
    float screenHeight;
    Camera2D camera;
    Rectangle cameraBounds;
    std::vector<Rectangle> platformBounds;
    std::vector<Rectangle> enemyObstacleBounds;
    Player player;
    std::vector<Enemy> enemies;
    Sound actionSound;
    Music music;
};