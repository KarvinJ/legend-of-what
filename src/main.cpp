#include <raylib.h>
#include "Player.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

bool isGamePaused;

Sound hitSound;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Legend-of-what");
    SetTargetFPS(60);

    Player player = Player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "assets/img/alien.png");

    InitAudioDevice(); // Initialize audio device, before loading sound and music.

    hitSound = LoadSound("assets/sounds/okay.wav");
    Music music = LoadMusicStream("assets/music/pixel3.mp3");

    music.looping = true;

    PlayMusicStream(music);

    while (!WindowShouldClose())
    {
        // UpdateMusicStream(music);

        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE))
        {
            isGamePaused = !isGamePaused;
            PlaySound(hitSound);
        }

        if (!isGamePaused)
        {
            player.Update(deltaTime);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextFormat("%i", player.score), 230, 20, 80, WHITE);

        player.Draw();

        if (isGamePaused)
        {
            DrawText("Game Paused", 220, 100, 80, WHITE);
        }

        EndDrawing();
    }

    player.Dispose();

    // Unload sound data
    UnloadSound(hitSound);
    UnloadMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
}