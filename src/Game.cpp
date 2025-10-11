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

    // music = LoadMusicStream("assets/music/pixel3.mp3");
    // music.looping = true;
    // PlayMusicStream(music);
}

void Game::Update()
{
    // UpdateMusicStream(music);

    float deltaTime = GetFrameTime();

    if (IsKeyPressed(KEY_SPACE))
    {
        isGamePaused = !isGamePaused;
        PlaySound(actionSound);
    }

    if (!isGamePaused)
    {
        player.Update(deltaTime);
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    player.Draw();

    if (isGamePaused)
    {
        DrawText("Game Paused", screenWidth / 2 - 200, 100, 80, WHITE);
    }

    EndDrawing();
}

Game::~Game()
{
    player.Dispose();
    UnloadSound(actionSound);
    // UnloadMusicStream(music);
    CloseAudioDevice();
}
