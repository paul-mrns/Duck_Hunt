/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.hpp
** Dog animation
*/

#pragma once
#include "AAnimation.hpp"


namespace Animation {
    enum class DogState { Sniffing, Jump, Happy1, Happy2, Laughing, Finished };
    class Dog : public AAnimation {
        public:
            Dog(const sf::Texture& texture);

            void happy1(sf::Vector2f pos);
            void update(float dt) override;
            bool isAnimationDone() const { return _state == DogState::Finished; }
            void initFramesRects();

        private:
            DogState _state = DogState::Sniffing;
            std::size_t _frameIndex = 0;
            float _elapsed = 0.f;
            float _totalElapsed = 0.f;
            float _speed = 0.15f;
            
            //sniffing
            std::vector<sf::IntRect> _sniffFrames;
            std::vector<int> _sniffOrder = {0, 3, 2, 1, 1, 4};

            //jump
            std::vector<sf::IntRect> _jumpFrames;
            std::vector<int> _jumpOrder  = {0, 1, 2, 2};
            std::vector<float> _jumpHeights = { 0.f, -120.f, -160.f, 0.f};
            std::vector<float> _jumpFrameDurations = {0.2f, 0.4f, 0.2f, 0.4f};
            sf::Vector2f _jumpStartPos;
            float _jumpSpeed = 0.05f;
            float _jumpTime = 0.f;
            float _jumpDuration = 1.2f;
            float _jumpDelay = 5.6f;

            //happy1
            sf::IntRect _happy1Frame;
            float _riseSpeed = 100.f;
            float _targetY = 600.f;
            bool _rising = false;
    };
}
