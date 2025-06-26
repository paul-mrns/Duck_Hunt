/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "assets/PlayAssets.hpp"

Assets::PlayAssets::PlayAssets(std::vector<int> values)
{
    if (values.size() != 5)
        return;
    _round = values[0];
    _reload = values[1];
    _ammo = values[2];
    _hits = values[3];
    _score = values[4];
    _font.loadFromFile(PLAY_FONT_PATH);
    visualAssets();
    roundAssets();
    reloadAssets();
    ammoAssets();
    hitAssets();
    scoreAssets();
}

void Assets::PlayAssets::roundAssets()
{
    _roundText.setFont(_font);
    _roundText.setString("ROUND\n  " + std::to_string(_round));
    sf::FloatRect roundBounds = _roundText.getLocalBounds();
    _roundText.setPosition((WINDOW_WIDTH - roundBounds.width) / 2.f - roundBounds.left, 200.0f);
    _roundText.setFillColor(sf::Color::White);
    _roundBg.setPosition(875.0f, 190.0f);
    _roundBg.setFillColor(sf::Color::Black);
    _roundBg.setOutlineThickness(3.0f);
    _roundBg.setOutlineColor(sf::Color::White);
    _roundBg.setSize((sf::Vector2f){170.0f, 75.0f});
}

void Assets::PlayAssets::reloadAssets()
{
    _reloadText.setFont(_font);
    _reloadText.setString("R=" + std::to_string(_reload));
    _reloadText.setPosition(100.0f, 930.0f);
    _reloadText.setFillColor(_green);
    sf::FloatRect reloadBounds = _reloadText.getGlobalBounds();
    _reloadBg.setPosition(reloadBounds.left, reloadBounds.top);
    _reloadBg.setFillColor(sf::Color::Black);
    _reloadBg.setSize((sf::Vector2f){reloadBounds.width + 3, reloadBounds.height + 3});
}

void Assets::PlayAssets::ammoAssets()
{
    _ammoText.setFont(_font);
    _ammoText.setString("SHOT");
    _ammoText.setCharacterSize(20.0f);
    _ammoText.setPosition(260.0f, 960.0f);
    _ammoText.setFillColor(_blue);
    _ammoBg.setSize((sf::Vector2f){97.0f, 60.0f});
    _ammoBg.setFillColor(sf::Color::Black);
    _ammoBg.setOutlineThickness(3.0f);
    _ammoBg.setOutlineColor(_green);
    _ammoBg.setPosition(250.0f, 930.0f);
    _bulletSpr.setTexture(_spritesheet);
    _bulletSpr.setTextureRect(sf::IntRect(0, 550, 25, 30));
    for (int i = 0; i < 3; i++) {
        sf::Sprite bullet(_bulletSpr);
        bullet.setPosition(260.0f + i * 30.0f, 927.0f);
        _bullets.push_back(bullet);
    }
}

void Assets::PlayAssets::hitAssets()
{
    _redDuckSpr.setTexture(_spritesheet);
    _redDuckSpr.setTextureRect(sf::IntRect(25, 550, 25, 30));
    _whiteDuckSpr.setTexture(_spritesheet);
    _whiteDuckSpr.setTextureRect(sf::IntRect(50, 550, 25, 30));
    _hitText.setFont(_font);
    _hitText.setString("HIT");
    _hitText.setPosition(1200.0f, 947.0f);
    _hitText.setFillColor(_green);
    _hitBg.setSize((sf::Vector2f){475.0f, 60.0f});
    _hitBg.setFillColor(sf::Color::Black);
    _hitBg.setOutlineThickness(3.0f);
    _hitBg.setOutlineColor(_green);
    _hitBg.setPosition(1175.0f, 930.0f);
    for (int i = 0; i < 10; i++) {
        sf::Sprite redDuck(_redDuckSpr);
        sf::Sprite whiteDuck(_whiteDuckSpr);

        redDuck.setPosition(1325.0f + i * 30.0f, 945.0f);
        whiteDuck.setPosition(1325.0f + i * 30.0f, 945.0f);
        _redDucks.push_back(redDuck);
        _whiteDucks.push_back(whiteDuck);
    }
}

static std::string alignString(int score)
{
    std::string scorestring = std::to_string(score);

    if (scorestring.size() >= 6)
        return scorestring;
    size_t zerosNeeded = 6 - scorestring.size();
    scorestring.insert(0, zerosNeeded, '0');
    return scorestring;
}

void Assets::PlayAssets::scoreAssets()
{
    _scoreText.setFont(_font);
    _scoreText.setString(alignString(_score) + "\n SCORE");
    _scoreText.setCharacterSize(25.0f);
    _scoreText.setPosition(1710.0f, 935.0f);
    _scoreBg.setSize((sf::Vector2f){175.0f, 60.0f});
    _scoreBg.setFillColor(sf::Color::Black);
    _scoreBg.setOutlineThickness(3.0f);
    _scoreBg.setOutlineColor(_green);
    _scoreBg.setPosition(1700.0f, 930.0f);
}

void Assets::PlayAssets::visualAssets()
{
    _backgroundText.loadFromFile("assets/play/background.png");
    _backgroundSpr.setTexture(_backgroundText);
    resizeSprite(_backgroundSpr, _backgroundText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _backgroundSpr.setPosition(0, 0);
    _spritesheet.loadFromFile("assets/play/spriteSheet.png");
}

void Assets::PlayAssets::setRound(int round)
{
    _round = round;
}

void Assets::PlayAssets::setReload(int reload)
{
    _reload = reload;
    _reloadText.setString("R=" + std::to_string(_reload));
}

void Assets::PlayAssets::setAmmo(int ammo)
{
    _ammo = ammo;
}

void Assets::PlayAssets::setHits(int hits)
{
    _hits = hits;
}

void Assets::PlayAssets::setScore(int score)
{
    _score = score;
    _scoreText.setString(alignString(_score) + "\n SCORE");
}