/*
** Paul Mourens project
** Duck Hunt cpp
** Duck.hpp
** Ducks animation
*/

#pragma once
#include "AAnimation.hpp"

#define SPRITE_WIDTH 77
#define SPRITE_HEIGTH 77

enum DuckColor { Red, Green, Blue };
enum DuckState { Flying, Hit, Falling, FlyAway, Caught };
enum DuckFlight { Horizontal, Diagonal, Up };


namespace Animation {
    class Duck : public AAnimation {
    public:
        Duck(const sf::Texture& texture);

        void update(float dt);
        void setDirection(sf::Vector2f dir);
        void isHit();

        DuckState getState() const;
        bool isOffScreen(const sf::RenderWindow& window) const;
        bool isFalling() const;
        bool isCaught() const;

    private:
        void initFrames();
        void applyAnimationFrame(float dt);
        void setFallFrames();
        void setFlyAwayFrames();
        void flipIfNeeded();
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

        float _speed;
        bool _isFlipped = false;

    };
}
