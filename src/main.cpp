#include "Game.h"

int main()
{
    Game game;

    while (!WindowShouldClose())
    {
        game.Update();
        game.Draw();
    }
}