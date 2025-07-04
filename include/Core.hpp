/*
** Paul Mourens project
** Duck Hunt cpp
** Core.hpp
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
#include <SFML/Audio.hpp>

#include "Types.hpp"
#include "Audio.hpp"
#include "assets/AAssets.hpp"
#include "assets/MenuAssets.hpp"
#include "assets/PlayAssets.hpp"
#include "animation/Dog.hpp"
#include "animation/Duck.hpp"
#include "game_states/Menu.hpp"
#include "game_states/Play.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define ERROR 84
#define MAX_HIGHSCORE_ENTRIES 10
#define HIGHSCORE_FILE ".secrets/scores.txt"

namespace DuckHunt
{
    class Audio;
    class Menu;
    class Play;
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
        gamemode _gamemode;
        Audio _audio;

        //gamestates
        std::unique_ptr<DuckHunt::Menu> _menu;
        std::unique_ptr<DuckHunt::Play> _play;

        // Score
        int _score = 0;
        std::vector<PlayerScore> _highScores;

        void loadHighScores(const std::string& filename);
        void gameLoop();
        void handleInput(sf::Event event);
        
        //stateHandler
        void stateHandler();
        void menuHandler();
        void playHandler();
    };
}