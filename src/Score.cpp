/*
** Paul Mourens project
** Duck Hunt cpp
** Score.cpp
*/

#include "Core.hpp"

static bool compareScore(const DuckHunt::PlayerScore& a, const DuckHunt::PlayerScore& b)
{
    return a.score > b.score;
}

void DuckHunt::Core::loadHighScores(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open high score file" << std::endl;
        _highScores.push_back({"", 0});
        return;
    }
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;

        if (std::getline(iss, name, ':')) {
            iss >> score;
            name.erase(name.find_last_not_of(" \t") + 1);
            name.erase(0, name.find_first_not_of(" \t"));
            _highScores.push_back({name, score});
        }
    }
    std::sort(_highScores.begin(), _highScores.end(), compareScore);
    if (_highScores.empty())
        _highScores.push_back({"", 0});
    file.close();
}
