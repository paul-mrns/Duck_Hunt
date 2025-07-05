/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.hpp
** Dog animation
*/

#pragma once
#include "AAnimation.hpp"

namespace Animation {
    enum class DogState { Sniffing, Jump, Happy1, Happy2, Laughing, GoLaughing, Finished };
    enum class CatchPhase { Rising, Holding, Falling };
    enum class LaughPhase { Rising, Standing, Falling };
    enum class GoLaughPhase { Waiting, Rising, Standing };
    class Dog : public AAnimation {
        public:
            Dog(const sf::Texture& texture);

            void initHappy1(sf::Vector2f pos);
            void initLaugh(sf::Vector2f pos);
            void initGameoverLaugh(sf::Vector2f pos);
            void update(float dt) override;
            bool isAnimationDone() const { return _state == DogState::Finished; }
            bool isFalling() const { return _state == DogState::Jump && _jumpPhase == 2; };

        private:
            DogState _state = DogState::Sniffing;
            std::size_t _frameIndex = 0;
            float _elapsed = 0.f;
            float _totalElapsed = 0.f;
            float _speed = 0.15f;
            
            void initFramesRects();

            //sniffing
            std::vector<sf::IntRect> _sniffFrames;
            std::vector<int> _sniffOrder = {0, 3, 2, 1, 1, 4};

            //jump
            std::vector<sf::IntRect> _jumpFrames;
            sf::Vector2f _jumpStartPos;
            float _jumpDelay = 5.5f;
            float _jumpTimer = 0.f;
            int _jumpPhase = 0;
            void jump(float dt);

            //Duck Catching
            sf::IntRect _happy1Frame;
            float _catchTimer = 0.f;
            CatchPhase _catchPhase = CatchPhase::Rising;
            void happy1(float dt);
            
            //Dog Laughing
            std::vector<sf::IntRect> _laughingFrames;
            float _laughTimer = 0.f;
            LaughPhase _laughPhase = LaughPhase::Rising;
            void laugh(float dt);

            //Dog gameover Laughing
            float _goLaughTimer = 0.f;
            GoLaughPhase _goLaughPhase = GoLaughPhase::Waiting;
            void gameoverLaugh(float dt);
    };
}
