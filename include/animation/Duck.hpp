/*
** Paul Mourens project
** Duck Hunt cpp
** Duck.hpp
** Ducks animation
*/

#pragma once
#include "AAnimation.hpp"

#define SPRITE_WIDTH 68
#define SPRITE_HEIGHT 62
#define SPRITE_SCALE 1.6f

#define BLUE_DUCK 0
#define BLACK_DUCK 1
#define RED_DUCK 2

enum DuckState { Flying, Hit, Falling, FlyAway, Caught };
enum DuckFlight { Horizontal, Diagonal, Up };

namespace Animation {
    class Duck : public AAnimation {
    public:
        Duck(const sf::Texture& texture, int round);

        void update(float dt);
        void setDirection(sf::Vector2f dir);
        void isHit();
        void flyAway();

        DuckState getState() const;
        bool isOffScreen() const;
        bool isFalling() const;
        void flyingAway() { return; };
        bool isCaught() const;
        int getDuckScore(int round) const;

    private:
        void initFrames();
        void applyAnimationFrame(float dt);
        void setFallFrames();
        float calculateSpeedFromRound(int round) const;
        void randomiseValues();
        std::vector<sf::IntRect> getFlyFrames(int color, DuckFlight fligth);

        sf::Vector2f _direction;
        int _color;
        DuckState _state;
        std::vector<sf::IntRect> _horizontalFlyFrames;
        std::vector<sf::IntRect> _diagonalFlyFrames;
        std::vector<sf::IntRect> _flyAwayFrames;
        sf::IntRect _hitFrame;
        sf::IntRect _fallFrame;
        sf::Clock _hitClock;

        //flyAway
        float _flyAwayElapsed = 0.f;
        float _flyAwaySpeed = 0.f;

        //falling 
        float _fallElapsed = 0.f;
        float _fallSpeed = 0.f;

        //flying
        float _flightTime = 0.f;
        bool _canLeaveScreen = false;
        float _speed;
        bool _isFlipped = false;
        int _currentFrame = 0;
        bool _hasLeftGrass = false;

        void duckFlying(float dt);
        bool bounce(sf::FloatRect bounds);
        void flipIfNeeded();
    };
}
