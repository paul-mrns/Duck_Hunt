/*
** Paul Mourens project
** Duck Hunt cpp
** Play.cpp
*/

#include "gameStates/Play.hpp"
#include "assets/PlayAssets.hpp"

DuckHunt::Play::Play(int &score, Audio& audio)
    : _audio(audio), _score(score)
{
    _assets = std::make_unique<Assets::PlayAssets>(std::vector<int>{1, 1, 3, 0, 0});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
}

void DuckHunt::Play::draw(sf::RenderWindow& window)
{
    window.draw(_assets->_backgroundSpr);
    if (!_gameHasStarted) {
        window.draw(_assets->_roundBg);
        window.draw(_assets->_roundText);
        if (_dog && !_gameHasStarted)
            _dog->draw(window);
    }
    window.draw(_assets->_reloadBg);
    window.draw(_assets->_reloadText);
    window.draw(_assets->_ammoBg);
    window.draw(_assets->_ammoText);
    window.draw(_assets->_hitBg);
    window.draw(_assets->_hitText);
    window.draw(_assets->_scoreBg);
    window.draw(_assets->_scoreText);
    drawAmmo(window, _assets->_bullets);
    drawHits(window, _assets->_redDucks, _assets->_whiteDucks);
}

void DuckHunt::Play::drawAmmo(sf::RenderWindow &window, std::vector<sf::Sprite> bullets)
{
    for (int i = 0; i < _ammo; i++)
        window.draw(bullets[i]);
}

void DuckHunt::Play::drawHits(sf::RenderWindow &window, std::vector<sf::Sprite> redDucks,
    std::vector<sf::Sprite> whiteDucks)
{
    for (int i = 0; i < _duckCount && i < 10; i++)
        window.draw(redDucks[i]);
    for (int i = _duckCount; i < 10; i++)
        window.draw(whiteDucks[i]);
}

void DuckHunt::Play::reload()
{
    _reload--;
    _ammo = 3;
    _assets->setReload(_reload);
    _assets->setAmmo(_ammo);
}

void DuckHunt::Play::handleInput(input &in)
{
    if (!_gameHasStarted)
        return;
    if (in == left_click) {
        _audio.play_sound(SHOT_SOUND);
        _ammo--;
        if (_ammo == 0) {
            if (_reload > 0)
                reload();
            else
                _gameOver = true;
        }
        return;
    } else
        return;
}

void DuckHunt::Play::update()
{
    float dt = _clock.restart().asSeconds();

    if (!_gameHasStarted) {
        if (_dog) {
            _dog->update(dt);
            if (_dog->isAnimationDone())
                _gameHasStarted = true;
        }
        return;
    }
}
