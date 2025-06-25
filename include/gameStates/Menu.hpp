/*
** Paul Mourens project
** Duck Hunt cpp
** Menu.hpp
*/

#pragma once
#include "Core.hpp"
#include "assets/MenuAssets.hpp"
#include "Types.hpp"

namespace Assets
{
    class MenuAssets;
}

namespace DuckHunt
{
    class Menu
    {
        public:
            Menu(const std::vector<DuckHunt::PlayerScore>& highScores);

            void handleInput(input &in);
            void update();
            void draw(sf::RenderWindow& window);
            bool isGameStartRequested() const;
            DuckHunt::gamemode gamemodeChosen() const;

        private:
            std::unique_ptr<Assets::MenuAssets> _assets;
            int _selectedOption;
            bool _startGame;
            const std::vector<DuckHunt::PlayerScore>& _highScores;
            bool _scoreboardActive;
            gamemode _gamemode;

            void updateSelection();
            void drawScores(sf::RenderWindow& window);
            gamemode getGamemode(int selection);
    };
}