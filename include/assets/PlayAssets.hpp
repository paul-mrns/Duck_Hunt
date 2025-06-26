/*
** Paul Mourens project
** Duck Hunt cpp
** PlayAssets.cpp
** assets handling: Play
*/

#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "AAssets.hpp"

#define PLAY_FONT_PATH "assets/fonts/PressStart2P-Regular.ttf"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Assets {
    class PlayAssets : public AAssets {
        public:
            PlayAssets(std::vector<int> values);
            ~PlayAssets() = default;

            sf::Color _green{137,243,54};
            sf::Color _blue{0, 204, 255};

            //visuals
            sf::Texture _backgroundText;
            sf::Sprite _backgroundSpr;
            sf::Texture _spritesheet;
            sf::Sprite _bulletSpr;
            sf::Sprite _redDuckSpr;
            sf::Sprite _whiteDuckSpr;
            std::vector<sf::Sprite> _bullets;
            std::vector<sf::Sprite> _redDucks;
            std::vector<sf::Sprite> _whiteDucks;

            //values background
            sf::RectangleShape _roundBg;
            sf::RectangleShape _reloadBg;
            sf::RectangleShape _ammoBg;
            sf::RectangleShape _hitBg;
            sf::RectangleShape _scoreBg;

            //texts
            sf::Font _font;
            sf::Text _roundText;
            sf::Text _reloadText;
            sf::Text _ammoText;
            sf::Text _hitText;
            sf::Text _scoreText;

            //values
            int _round;
            int _reload;
            int _ammo;
            int _hits;
            int _score;

            //setters
            void setRound(int round);
            void setReload(int reload);
            void setAmmo(int ammos);
            void setHits(int hits);
            void setScore(int score);

            //init 
            void visualAssets();
            void roundAssets();
            void reloadAssets();
            void ammoAssets();
            void hitAssets();
            void scoreAssets();
    };
}
