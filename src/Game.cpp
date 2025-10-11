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
    for (auto &structure : collisionBounds) {

            if (CheckCollisionRecs(player.bounds, structure)) {

//                If the player previous position is within the x bounds of the platform,
//                then we need to resolve the collision by changing the y value
                if (CheckCollisionInX(player.GetPreviousPosition(), structure)) {

//                    Player was falling downwards. Resolve upwards.
                    if (player.velocity.y < 0) {

                        player.bounds.y = structure.y + structure.height;
                        player.velocity.y = 0;

                        // if (player.velocity.y == 0 && Gdx.input.isKeyPressed(Input.Keys.SPACE))
                        //     player.velocity.y = 800 * deltaTime;
                    }

//                     Player was moving upwards. Resolve downwards
                    else {

                        //check this case. For the collision bug
                        player.bounds.y = structure.y - player.bounds.height;
                        player.velocity.y = 0;
                    }
                }
                //  If the player previous position is within the y bounds of the platform,
//                then we need to resolve the collision by changing the x value
                else if (CheckCollisionInY(player.GetPreviousPosition(), structure)) {

//                     Player was traveling right. Resolve to the left
                    if (player.velocity.x > 0)
                        player.bounds.x = structure.x - player.bounds.width;

//                     Player was traveling left. Resolve to the right
                    else
                        player.bounds.x = structure.x + structure.width;

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
}
