#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

unordered_map<string, Rectangle> loadSpriteSheetData(string path);

vector<string> customSplit(string &str, char separator);