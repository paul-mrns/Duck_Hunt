/*
** Paul Mourens project
** Duck Hunt cpp
** Play.hpp
*/

#pragma once

#include "../assets/PlayAssets.hpp"
#include "Types.hpp"
#include "Audio.hpp"
#include "../animation/Dog.hpp"
#include "../animation/Duck.hpp"

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

        int _round = 1;
        int _reload = 1;
        int _ammo = 3;
        int _duckCount = 0;
        int& _score;
        bool _gameOver = false;
        bool _gameHasStarted = false;

        void playLoop(float dt);

        //drawing
        void drawAmmo(sf::RenderWindow& window, std::vector<sf::Sprite> bullets);
        void drawHits(sf::RenderWindow& window, std::vector<sf::Sprite> redDucks, std::vector<sf::Sprite> whiteDucks);
        
        // Game mechanics
        void shoot(sf::Vector2i mousePos);
        void reload();
        void reloadUsed();

        // Duck
        std::unique_ptr<Animation::Duck> _duck;
        bool _duckActive = false;
        int _duckIndex = 0;
        sf::FloatRect _duckBounds;
        std::vector<hitResults> _duckHits;
        void spawnDuck();
        void duckHasBeenHit();

        //Dog
        std::unique_ptr<Animation::Dog> _dog;
        bool _dogCatching = false;
        bool _dogHasAppeared = false;
        void dogCatchingDuck(float dt);

        // Shooting
        sf::RectangleShape _blackScreen;
        sf::RectangleShape _hitboxRect;
        bool _showFlash = false;
        sf::Clock _flashClock;
        sf::Vector2f _lastMouseShot;
        void handleHit(sf::Vector2i mousePos);
    };
}
