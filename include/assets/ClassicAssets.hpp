/*
** Paul Mourens project
** Duck Hunt cpp
** ClassicAssets.cpp
** assets handling: Classic gamemode
*/

#pragma once
#include "Core.hpp"

#define PLAY_FONT_PATH "assets/fonts/PressStart2P-Regular.ttf"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Assets {
    class ClassicAssets : public AAssets {
        public:
            ClassicAssets(std::vector<int> values);
            ~ClassicAssets() = default;

            //visuals
            sf::Texture _backgroundText;
            sf::Sprite _backgroundSpr;
            sf::Texture _flyAwayBackgroundText;
            sf::Sprite _flyAwayBackgroundSpr;
            sf::Texture _grassText;
            sf::Sprite _grassSpr;
            sf::Texture _spritesheet;

            //Hud
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
            sf::Sprite _textBgSpr;
            sf::Texture _textBgText;
            sf::Sprite _roundBgSpr;
            sf::Texture _roundBgText;

            //values background
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
            sf::Text _hitScoreText;
            sf::Text _perfectRoundText;
            sf::Text _perfectRoundScoreText;
            sf::Text _requiredBar;
            sf::Text _gameoverText;

            //setters
            void setRound(int round);
            void setAmmo(int ammos);
            void setScore(int score);
            void setHitScore(int hitScore, sf::Vector2f pos);

        private:
            int _round;
            int _ammo;
            int _score;
            int _hitScore;
            int _perfectRoundBonus;
            sf::Color _green{137, 243, 54};
            sf::Color _blue{0, 204, 255};

            void visualAssets();
            void roundAssets();
            void ammoAssets();
            void hitAssets();
            void scoreAssets();
            void flyAwayAssets();
            void pauseAssets();
            void hitScoreAssets();
            void perfectRoundAssets();
            void gameOverAssets();
    };
}
