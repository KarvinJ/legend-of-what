#include "Game.h"

Game::Game()
{
    isGamePaused = false;
    screenWidth = 1280;
    screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Legend-of-what");
    SetTargetFPS(60);
    InitAudioDevice();

    actionSound = LoadSound("assets/sounds/okay.wav");

    Texture2D characterSpriteSheet = LoadTexture("assets/img/characters/character-spritesheet.png");
    unordered_map<string, Rectangle> characterSpriteSheetData = loadSpriteSheetData("assets/img/characters/character-spritesheet.txt");

    player = Player(screenWidth / 2, screenHeight / 2, characterSpriteSheet, characterSpriteSheetData);

    Texture2D enemiesSpriteSheet = LoadTexture("assets/img/enemies/enemies-spritesheet.png");
    unordered_map<string, Rectangle> enemiesSpriteSheetData = loadSpriteSheetData("assets/img/enemies/enemies-spritesheet.txt");
    enemy = Enemy(screenWidth / 2 + 150, 518, enemiesSpriteSheet, enemiesSpriteSheetData);

    collisionBounds = {
        {0, screenHeight - 64, screenWidth, 128},
        {screenWidth / 2, screenHeight - 180, 64, 128},
        {screenWidth / 2 + 140, screenHeight - 180, 128, 64},
        {screenWidth / 2 - 200, screenHeight - 180, 128, 64},
    };

    camera = {0};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f + 100}; // this is for control the camera default position
    camera.target = {player.bounds.x, 600};                                   // the object that the camera will be following
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    // music = LoadMusicStream("assets/music/pixel3.mp3");
    // music.looping = true;
    // PlayMusicStream(music);
}

void Game::Update(float deltaTime)
{
    // UpdateMusicStream(music);

    if (IsKeyPressed(KEY_F))
    {
        isGamePaused = !isGamePaused;
        PlaySound(actionSound);
    }

    if (!isGamePaused)
    {
        player.Update(deltaTime);

        enemy.Update(deltaTime);

        if (player.actualState == Player::AnimationState::ATTACKING)
        {
            enemy.HasBeenHit(player.attackBounds);
        }

        ManageStructureCollision(deltaTime);
    }
}

void Game::Draw(float deltaTime)
{
    BeginDrawing();

    ClearBackground(BLACK);

    // all the code that should be affected by the camerea, should be put inside BeginMode2D
    BeginMode2D(camera);

    // And here I give the camera the player position for the camera to follow.
    camera.target = {player.bounds.x, 600};

    for (auto &collisionBound : collisionBounds)
    {
        DrawRectangleRec(collisionBound, BLUE);
    }

    player.Draw(deltaTime);

    if (!enemy.isDestroyed)
    {
        enemy.Draw(deltaTime);
    }

    EndMode2D();

    if (isGamePaused)
    {
        DrawText("Game Paused", screenWidth / 2 - 200, 100, 80, WHITE);
    }

    EndDrawing();
}

void Game::Run()
{
    float deltaTime = GetFrameTime();

    Update(deltaTime);
    Draw(deltaTime);
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
        Rectangle playerBounds = player.GetCollisionBounds();

        if (CheckCollisionRecs(playerBounds, platform))
        {
            if (CheckCollisionInX(player.GetPreviousPosition(), platform))
            {
                if (player.velocity.y > 0)
                {
                    player.bounds.y = platform.y - playerBounds.height;
                    player.velocity.y = 0;

                    if (IsKeyPressed(KEY_SPACE))
                    {
                        player.velocity.y = -550 * deltaTime;
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
                    player.bounds.x = platform.x - playerBounds.width;
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
    enemy.Dispose();
    UnloadSound(actionSound);
    // UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}
