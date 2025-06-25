/*
** Paul Mourens project
** Duck Hunt cpp
** Menu.cpp
** assets handling: Menu
*/

#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "AAssets.hpp"

#define MENU_FONT_PATH "assets/fonts/menu_font.ttf"
#define LOGO_WIDTH 700.0f
#define LOGO_HEIGHT 550.0f

namespace DuckHunt {
    struct PlayerScore;
}

namespace Assets
{
    class MenuAssets : public AAssets
    {
        public:
            //logo
            sf::Texture _logoText;
            sf::Sprite _logoSpr;

            //texts
            sf::Font _font;
            std::vector<sf::Text> _games;
            sf::Text _scores;
            sf::Text _credits;
            sf::Text _topScore;

            //scoreboard
            sf::Text _highscoreText;
            std::vector<sf::Text> _highScores;
            sf::RectangleShape _scoreboard;
            void createScoreboard();
            void createHeader();
            void createPodium(const std::vector<DuckHunt::PlayerScore> &highscores);
            void createScores(const std::vector<DuckHunt::PlayerScore> &highscores);


            MenuAssets(const std::vector<DuckHunt::PlayerScore> &highscores);
            ~MenuAssets() = default;

            void textAssets(const std::vector<DuckHunt::PlayerScore> &highscores);
            void logoAssets();

    };
}
