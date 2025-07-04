/*
** Paul Mourens project
** Duck Hunt cpp
** PlayAssets.cpp
** assets handling: Play
*/

#pragma once
#include "Core.hpp"

#define PLAY_FONT_PATH "assets/fonts/PressStart2P-Regular.ttf"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Assets {
    class PlayAssets : public AAssets {
        public:
            PlayAssets(std::vector<int> values);
            ~PlayAssets() = default;

            //visuals
            sf::Texture _backgroundText;
            sf::Sprite _backgroundSpr;
            sf::Texture _grassText;
            sf::Sprite _grassSpr;
            sf::Texture _spritesheet;
            sf::Sprite _bulletSpr;
            sf::Sprite _redDuckSpr;
            sf::Sprite _whiteDuckSpr;
            std::vector<sf::Sprite> _bullets;
            std::vector<sf::Sprite> _redDucks;
            std::vector<sf::Sprite> _whiteDucks;
            sf::Sprite _pauseSpr;
            sf::Texture _pauseText;
            sf::Sprite _flyAwaySpr;
            sf::Texture _flyAwayText;

            //values background
            sf::RectangleShape _roundBg;
            sf::RectangleShape _greenRoundBg;
            sf::RectangleShape _ammoBg;
            sf::RectangleShape _hitBg;
            sf::RectangleShape _scoreBg;

            //texts
            sf::Font _font;
            sf::Text _roundText;
            sf::Text _greenRoundText;
            sf::Text _ammoText;
            sf::Text _hitText;
            sf::Text _scoreText;

            //setters
            void setRound(int round);
            void setAmmo(int ammos);
            void setHits(int hits);
            void setScore(int score);

        private:
            int _round;
            int _ammo;
            int _hits;
            int _score;
            sf::Color _green{137,243,54};
            sf::Color _blue{0, 204, 255};

            void visualAssets();
            void roundAssets();
            void ammoAssets();
            void hitAssets();
            void scoreAssets();
            void flyAwayAssets();
            void pauseAssets();
    };
}
