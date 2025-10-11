#include "Game.h"

Game::Game()
{
    isGamePaused = false;
    screenWidth = 1280;
    screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Legend-of-what");
    SetTargetFPS(60);
    InitAudioDevice();

    player = Player(screenWidth / 2, screenHeight / 2, "assets/img/alien.png");
    actionSound = LoadSound("assets/sounds/okay.wav");

    collisionBounds = {
        {0, screenHeight - 64, screenWidth, 64},
        {screenWidth / 2, screenHeight - 180, 64, 128},
        {screenWidth / 2 + 140, screenHeight - 200, 128, 64},
        {screenWidth / 2 - 200, screenHeight - 200, 128, 64},
    };

    // music = LoadMusicStream("assets/music/pixel3.mp3");
    // music.looping = true;
    // PlayMusicStream(music);
}

void Game::Update()
{
    // UpdateMusicStream(music);

    float deltaTime = GetFrameTime();

    if (IsKeyPressed(KEY_F))
    {
        isGamePaused = !isGamePaused;
        PlaySound(actionSound);
    }

    if (!isGamePaused)
    {
        player.Update(deltaTime);

        ManageStructureCollision(deltaTime);
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    for (auto &collisionBound : collisionBounds)
    {
        DrawRectangleRec(collisionBound, BLUE);
    }

    player.Draw();

    if (isGamePaused)
    {
        DrawText("Game Paused", screenWidth / 2 - 200, 100, 80, WHITE);
    }

    EndDrawing();
}

bool Game::CheckCollisionInX(Rectangle bounds, Rectangle platform)
{
    return bounds.x + bounds.width > platform.x && bounds.x < platform.x + platform.width;
}

bool Game::CheckCollisionInY(Rectangle bounds, Rectangle platform)
{
    return bounds.y + bounds.height > platform.y && bounds.y < platform.y + platform.height;
}

void Game::ManageStructureCollision(float deltaTime)
{
    for (auto &platform : collisionBounds)
    {
        if (CheckCollisionRecs(player.bounds, platform))
        {
            if (CheckCollisionInX(player.GetPreviousPosition(), platform))
            {
                if (player.velocity.y > 0)
                {
                    player.bounds.y = platform.y - player.bounds.height;
                    player.velocity.y = 0;

                    if (IsKeyPressed(KEY_SPACE))
                    {
                        player.velocity.y = -600 * deltaTime;
                    }
                }

                else
                {
                    // check this case. For the collision bug
                    player.bounds.y = platform.y + platform.height;
                    player.velocity.y = 0;
                }
            }
            else if (CheckCollisionInY(player.GetPreviousPosition(), platform))
            {
                if (player.velocity.x > 0)
                {
                    player.bounds.x = platform.x - player.bounds.width;
                }

                else
                {
                    player.bounds.x = platform.x + platform.width;
                }

                player.velocity.x = 0;
            }
        }
    }
}

Game::~Game()
{
    player.Dispose();
    UnloadSound(actionSound);
    // UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}
