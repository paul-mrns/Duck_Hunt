/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "gameStates/Menu.hpp"
#include "Types.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGTH 1080
#define ERROR 84
#define MAX_HIGHSCORE_ENTRIES 10
#define HIGHSCORE_FILE ".secrets/scores.txt"

namespace DuckHunt
{
    class Menu;
    class Core
    {
    public:
        Core(std::string &username);
        ~Core() = default;

    private:
        game_state _state;
        std::string _username;
        sf::RenderWindow _window;
        input _input;
        std::unique_ptr<DuckHunt::Menu> _menu;

        // Score
        int _score;
        int _topScore;
        std::vector<PlayerScore> _highScores;

        void loadHighScores(const std::string& filename);
        void gameLoop();
        void handleInput(sf::Event event);
        void stateHandler();
    };
}