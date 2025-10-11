#pragma once
#include <raylib.h>
#include "Player.h"

class Game
{

public:
    Game();
    ~Game();
    void Update();
    void Draw();

private:
    bool isGamePaused;
    int screenWidth;
    int screenHeight;
    Player player;
    Sound actionSound;
    Music music;
};