/*
** Paul Mourens project
** Duck Hunt cpp
** Menu.hpp
*/

#pragma once
#include "Core.hpp"

namespace Assets
{
    class MenuAssets;
}

namespace DuckHunt
{
    class Menu
    {
        public:
            Menu(const std::vector<DuckHunt::PlayerScore>& highScores, DuckHunt::Audio& audio);

            void handleInput(input &in, sf::Vector2i mousePos);
            void update();
            void draw(sf::RenderWindow& window);
            bool isGameStartRequested() const;
            void setStartGame(bool isGameStarted) { _startGame = isGameStarted; };
            DuckHunt::gamemode gamemodeChosen() const;

        private:
            std::unique_ptr<Assets::MenuAssets> _assets;
            int _selectedOption;
            bool _startGame = false;
            const std::vector<DuckHunt::PlayerScore>& _highScores;
            bool _scoreboardActive = false;
            gamemode _gamemode;
            DuckHunt::Audio& _audio;

            void updateSelection();
            void drawScores(sf::RenderWindow& window);
            gamemode getGamemode(int selection);
            void handleClick(sf::Vector2i mousePos);
            void handleHitboxes(sf::Vector2i mousePos);
    };
}