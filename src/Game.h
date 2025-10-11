#pragma once
#include <raylib.h>
#include "Player.h"

class Game
{

public:
    Game();
    void Update();
    void Draw();
    void Dispose();

private:
    Player player;
    bool isGamePaused;
    int screenWidth;
    int screenHeight;
    Sound actionSound;
    Music music;
};