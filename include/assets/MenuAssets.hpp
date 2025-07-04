/*
** Paul Mourens project
** Duck Hunt cpp
** MenuAssets.cpp
** assets handling: Menu
*/

#pragma once
#include "Core.hpp"

#define MENU_FONT_PATH "assets/fonts/PressStart2P-Regular.ttf"
#define LOGO_WIDTH 900.f
#define LOGO_HEIGHT 550.f

namespace DuckHunt {
    struct PlayerScore;
}

namespace Assets
{
    class MenuAssets : public AAssets
    {
        public:
            MenuAssets(const std::vector<DuckHunt::PlayerScore> &highscores);
            ~MenuAssets() = default;
            
            //logo
            sf::Sprite _logoSpr;

            //texts
            std::vector<sf::Text> _games;
            sf::Text _scores;
            sf::Text _credits;
            sf::Text _topScore;

            //scoreboard
            sf::Text _highscoreText;
            std::vector<sf::Text> _highScores;
            sf::RectangleShape _scoreboard;

        private:
            sf::Font _font;
            sf::Texture _logoText;

            void createScoreboard();
            void createHeader();
            void createPodium(const std::vector<DuckHunt::PlayerScore> &highscores);
            void createScores(const std::vector<DuckHunt::PlayerScore> &highscores);

            void textAssets(const std::vector<DuckHunt::PlayerScore> &highscores);
            void logoAssets();
    };
}
