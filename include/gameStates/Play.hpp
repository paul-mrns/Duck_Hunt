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

namespace DuckHunt
{
    class Play
    {
    public:
        Play(int &score, Audio &audio);
        ~Play() = default;

        void handleInput(input &in);
        void update();
        void draw(sf::RenderWindow& window);
        bool isGameOver() const { return _gameOver; };

    private:
        std::unique_ptr<Assets::PlayAssets> _assets;
        DuckHunt::Audio& _audio;
        sf::Clock _clock;
        std::unique_ptr<Animation::Dog> _dog;

        int _round = 1;
        int _reload = 1;
        int _ammo = 3;
        int _duckCount = 0;
        int& _score;
        bool _gameOver = false;
        bool _gameHasStarted = false;

        void drawAmmo(sf::RenderWindow& window, std::vector<sf::Sprite> bullets);
        void drawHits(sf::RenderWindow& window, std::vector<sf::Sprite> redDucks, std::vector<sf::Sprite> whiteDucks);
        void reload();
    };
}
