/*
** Paul Mourens project
** Duck Hunt cpp
** Play.hpp
*/

#pragma once
#include "Core.hpp"

namespace DuckHunt
{
    enum hitResults { Missed, Hit, Blinking };
    class Play
    {
    public:
        Play(int &score, Audio &audio);
        ~Play() = default;

        void handleInput(input &in, sf::Vector2i mousePos);
        void update();
        void draw(sf::RenderWindow& window);
        bool isGameOver() const { return _gameOver; };

    private:
        std::unique_ptr<Assets::PlayAssets> _assets;
        DuckHunt::Audio& _audio;
        sf::Clock _clock;

        void playLoop(float dt);

        bool _pause = false;
        int _round = 1;
        int _ammo = 3;
        int& _score;
        bool _gameOver = false;
        bool _gameHasStarted = false;

        //current duck
        int _duckIndex = 0;
        std::vector<hitResults> _duckHits;
        float _currentDuckBlinkTimer = 0.f;
        bool _currentDuckBlinking = true;

        //drawing
        void drawAmmo(sf::RenderWindow& window, std::vector<sf::Sprite> bullets);
        void drawHits(sf::RenderWindow& window, std::vector<sf::Sprite> redDucks, std::vector<sf::Sprite> whiteDucks);
        void drawIntro(sf::RenderWindow& window);
        void drawHUD(sf::RenderWindow& window);

        // Game mechanics
        void shoot(sf::Vector2i mousePos);
        void reload();

        // Duck
        std::unique_ptr<Animation::Duck> _duck;
        bool _duckActive = false;
        sf::FloatRect _duckBounds;
        bool _flyAway = false;
        void spawnDuck();
        void duckHasBeenHit();
        void duckHasBeenCaught();
        bool isDuckHit();

        //Dog
        std::unique_ptr<Animation::Dog> _dog;
        bool _dogCatching = false;
        bool _dogLaughing = false;
        bool _dogHasAppeared = false;
        void dogCatchingDuck(float dt);
        void dogLaughing();

        // Shooting
        sf::RectangleShape _blackScreen;
        sf::RectangleShape _hitboxRect;
        bool _showFlash = false;
        sf::Clock _flashClock;
        sf::Vector2f _lastMouseShot;
        void handleHit(sf::Vector2i mousePos);

        // points counting
        bool _pointsCounting = false;
        float _pointsCountingTimer = 0.f;
        size_t _pointsCountingIndex = 0;
        void pointsCountingSequence(float dt);

        // gameOver
        bool _gameOverSequence = false;
        bool _gameOverAnimationStarted = false;
        void gameOverLaugh(float dt);
        float _gameOverTimer = 0.f;

        //end round
        bool _endGame = false;
        bool _newRound = false;
        bool isPerfectRound();
        bool canStartNewRound();
        void startNewRound();

    };
}
