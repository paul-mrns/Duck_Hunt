/*
** Paul Mourens project
** Duck Hunt cpp
** Play.hpp
*/

#pragma once
#include "Core.hpp"
#define MAX_DUCKS_PER_ROUND 10

namespace DuckHunt
{
    enum hitResults { Missed, Hit, Blinking };
    class Classic
    {
    public:
        Classic(int &score, Audio &audio, int round, int ducks);
        ~Classic() = default;

        void handleInput(input &in, sf::Vector2i mousePos);
        void update();
        void draw(sf::RenderWindow& window);
        bool isGameOver() const { return _gameOver; };
        bool isNewRoundStart() const { return _newRoundCanStart; };

    private:
        std::unique_ptr<Assets::ClassicAssets> _assets;
        DuckHunt::Audio& _audio;
        sf::Clock _clock;

        void playLoop(float dt);

        bool _pause = false;
        int _round;
        int _ammo = 3;
        int& _score;
        bool _gameOver = false;
        bool _gameHasStarted = false;
        int _maxDucksOnScreen;

        //current duck
        int _duckIndex = 0;
        int _ducksHit = 0;
        std::vector<hitResults> _duckHits;
        float _currentDuckBlinkTimer = 0.f;
        bool _currentDuckBlinking = true;

        //drawing
        void drawAmmo(sf::RenderWindow& window, std::vector<sf::Sprite> bullets);
        void drawHits(sf::RenderWindow& window, std::vector<sf::Sprite> redDucks, std::vector<sf::Sprite> whiteDucks);
        void drawIntro(sf::RenderWindow& window);
        void drawHUD(sf::RenderWindow& window);

        // Game mechanics
        bool canShoot();
        void shoot(sf::Vector2i mousePos);
        void reload();

        // Duck
        std::vector<std::unique_ptr<Animation::Duck>> _ducks;
        bool _flyAway = false;
        float _duckSpawnTimer = 0.f;
        std::pair<int, int> _caughtOrder = {-1, -1};
        void spawnDuck(float dt);
        void duckHasBeenHit(int duckIndex);
        void duckHasBeenCaught();
        sf::Vector2f getLastCaughtPos(int ducksCaught);
        bool ducksAllLeft();
        bool isDuckHit(size_t duckIndex);

        //Dog
        std::unique_ptr<Animation::Dog> _dog;
        bool _dogCatching = false;
        bool _dogLaughing = false;
        bool _dogHasAppeared = false;
        void dogCatchingDuck(float dt);
        void dogLaughing();

        // Shooting
        sf::RectangleShape _blackScreen;
        std::vector<sf::RectangleShape> _hitboxRects;
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
        bool isPerfectRound();
        int getPerfectRoundBonus(int round);

        //Round outro
        float _jingleTimer = 0.f;
        bool _jingleStarted = false;
        bool _ducksFlickering = true;
        bool _perfectBonusStarted = false;
        float _perfectTimer = 0.f;
        float _flickerTimer = 0.f;

        //new round
        bool _newRoundCanStart = false;
        bool _newRound = false;
        bool _perfectRound = false;
        bool canStartNewRound();
        void newRoundOutro(float dt);
    };
}
