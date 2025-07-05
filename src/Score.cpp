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

void DuckHunt::Core::loadHighScores()
{
    std::ifstream file(HIGHSCORE_FILE);
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

void DuckHunt::Core::updateScoresList()
{
    std::ofstream outFile(HIGHSCORE_FILE);

    if (!outFile.is_open()) {
        std::cerr << "Failed to write high score file" << std::endl;
        return;
    }
    for (const auto& entry : _highScores)
        outFile << entry.username << " : " << entry.score << "\n";
    outFile.close();
}

void DuckHunt::Core::saveScore()
{
    bool existingScore = false;

    for (auto& currentScore : _highScores) {
        if (currentScore.username == _username && _score > currentScore.score) {
            currentScore.score = _score;
            existingScore = true;
            break;
        }
    }
    if (!existingScore)
        _highScores.push_back({_username, _score});
    std::sort(_highScores.begin(), _highScores.end(), compareScore);
    updateScoresList();
}
