#pragma once
#include <raylib.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef struct
{
    string name;
    Rectangle bounds;
} TextureInfo;

vector<TextureInfo> loadSpriteSheet(string path);

vector<string> customSplit(string &str, char separator);

TextureInfo getTextureInfoByName(vector<TextureInfo> textureInfos, string name);