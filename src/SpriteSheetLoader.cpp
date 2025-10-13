#include "SpriteSheetLoader.h"
#include <fstream>

unordered_map<string, Rectangle> loadSpriteSheetData(string path)
{
    unordered_map<string, Rectangle> spriteSheetData;
    spriteSheetData.reserve(4);

    std::ifstream spriteDataFile(path);

    for (string line; getline(spriteDataFile, line);)
    {
        auto list = customSplit(line, ',');

        string name = list[0];
        int x = stoi(list[1]);
        int y = stoi(list[2]);
        int width = stoi(list[3]);
        int height = stoi(list[4]);

        Rectangle bounds = {(float)x, (float)y, (float)width, (float)height};

        spriteSheetData[name] = bounds;
    }

    spriteDataFile.close();

    return spriteSheetData;
}

vector<string> customSplit(string &str, char separator)
{
    vector<string> strings;

    int startIndex = 0, endIndex = 0;
    for (size_t i = 0; i <= str.size(); i++)
    {
        if (str[i] == separator || i == str.size())
        {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }

    return strings;
}