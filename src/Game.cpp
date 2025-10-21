#include "Game.h"

Game::Game()
{
    isGamePaused = false;
    isDebugCamera = false;
    screenWidth = 1280;
    screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Legend-of-what");
    SetTargetFPS(60);
    InitAudioDevice();

    actionSound = LoadSound("assets/sounds/okay.wav");

    Texture2D characterSpriteSheet = LoadTexture("assets/img/characters/character-spritesheet.png");
    unordered_map<string, Rectangle> characterSpriteSheetData = loadSpriteSheetData("assets/img/characters/character-spritesheet.txt");

    player = Player(screenWidth / 2, 550, characterSpriteSheet, characterSpriteSheetData);

    Texture2D enemiesSpriteSheet = LoadTexture("assets/img/enemies/enemies-spritesheet.png");
    unordered_map<string, Rectangle> enemiesSpriteSheetData = loadSpriteSheetData("assets/img/enemies/enemies-spritesheet.txt");

    enemies = {
        Enemy(screenWidth / 2 - 200, 568, enemiesSpriteSheet, enemiesSpriteSheetData),
        Enemy(screenWidth / 2 + 150, 568, enemiesSpriteSheet, enemiesSpriteSheetData),
    };

    enemyObstacleBounds = {
        {screenWidth / 2 - 50, screenHeight - 150, 8, 16},
        {screenWidth / 2 - 300, screenHeight - 150, 8, 16},
        {screenWidth / 2 + 100, screenHeight - 150, 8, 16},
        {screenWidth / 2 + 350, screenHeight - 150, 8, 16},
    };

    float standardPlatformWidth = 256;

    float gapSize = 150;
    // float littlePlatformWidth = 128;
    platformBounds = {
        {0, screenHeight - 64, screenWidth, 128},
        {screenWidth / 2, screenHeight - 130, 64, 128},
        {screenWidth / 2 + 100, screenHeight - 130, standardPlatformWidth, 64},
        {screenWidth / 2 - 300, screenHeight - 130, standardPlatformWidth, 64},
    };

    for (size_t i = 1; i < 10; i++)
    {
        float xValue = gapSize * i + standardPlatformWidth * i-1;

        Rectangle platformBound = {xValue, screenHeight - 64, standardPlatformWidth, 128};
        platformBounds.push_back(platformBound);
    }

    cameraBounds = {screenWidth / 2.0f, screenHeight / 2.0f + 100, screenWidth, screenHeight};
    camera = {0};
    camera.offset = (Vector2){cameraBounds.x, cameraBounds.y}; // this is for control the camera default position
    camera.target = {player.bounds.x, 600};                    // the object that the camera will be following
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

        for (auto &enemy : enemies)
        {
            enemy.Update(deltaTime);

            for (auto &enemyObstacleBound : enemyObstacleBounds)
            {
                enemy.HasCollideWithObstacle(enemyObstacleBound);
            }

            if (!enemy.isDead)
            {
                player.HasBeenHit(enemy.GetCollisionBounds());

                if (player.actualState == Player::AnimationState::ATTACKING)
                {
                    enemy.HasBeenHit(player.attackBounds);
                }
            }
        }

        ManageStructureCollision(deltaTime);
    }
}

void Game::Draw(float deltaTime)
{
    BeginDrawing();

    ClearBackground(BLACK);

    // all the code that should be affected by the camerea, should be put inside BeginMode2D

    if (IsKeyPressed(KEY_F1))
    {
        isDebugCamera = !isDebugCamera;
    }

    if (isDebugCamera)
    {
        CameraController(camera);
    }

    BeginMode2D(camera);

    // And here I give the camera the player position for the camera to follow.
    camera.target = {player.bounds.x, 600};

    for (auto &collisionBound : platformBounds)
    {
        DrawRectangleRec(collisionBound, BLUE);
    }

    // for (auto &enemyObstacleBound : enemyObstacleBounds)
    // {
    //     DrawRectangleRec(enemyObstacleBound, YELLOW);
    // }

    player.Draw(deltaTime);

    for (auto &enemy : enemies)
    {
        if (!enemy.isDestroyed)
        {
            enemy.Draw(deltaTime);
        }
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
    for (auto &platform : platformBounds)
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
                        player.velocity.y = -425 * deltaTime;
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

void Game::CameraController(Camera2D &camera)
{
    if (IsKeyDown(KEY_RIGHT))
    {

        cameraBounds.x += 6;
    }
    else if (IsKeyDown(KEY_LEFT))
    {

        cameraBounds.x -= 6;
    }

    // Camera target follows camera bounds
    camera.target = (Vector2){cameraBounds.x, 600};

    // Camera zoom controls
    // Uses log scaling to provide consistent zoom speed
    camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

    if (camera.zoom > 3.0f)
    {
        camera.zoom = 3.0f;
    }
    else if (camera.zoom < 0.1f)
    {
        camera.zoom = 0.1f;
    }

    // Camera reset (zoom and rotation)
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 2.0f;
    }
}

Game::~Game()
{
    player.Dispose();
    // enemies.Dispose();
    UnloadSound(actionSound);
    // UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}
