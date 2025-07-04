/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "Core.hpp"

Assets::PlayAssets::PlayAssets(std::vector<int> values)
{
    if (values.size() != 4)
        return;
    _round = values[0];
    _ammo = values[1];
    _hits = values[2];
    _score = values[3];
    _font.loadFromFile(PLAY_FONT_PATH);
    visualAssets();
    roundAssets();
    ammoAssets();
    hitAssets();
    scoreAssets();
    pauseAssets();
    flyAwayAssets();
}

void Assets::PlayAssets::roundAssets()
{
    _roundText.setFont(_font);
    _roundText.setString("ROUND\n  " + std::to_string(_round));
    sf::FloatRect roundBounds = _roundText.getLocalBounds();
    _roundText.setPosition((WINDOW_WIDTH - roundBounds.width) / 2.f - roundBounds.left, 200.f);
    _roundText.setFillColor(sf::Color::White);
    _roundBg.setPosition(875.f, 190.f);
    _roundBg.setFillColor(sf::Color::Black);
    _roundBg.setOutlineThickness(3.f);
    _roundBg.setOutlineColor(sf::Color::White);
    _roundBg.setSize((sf::Vector2f){170.f, 75.f});
    _greenRoundText.setFont(_font);
    _greenRoundText.setString("R=" + std::to_string(_round));
    _greenRoundText.setPosition(100.f, 930.f);
    _greenRoundText.setFillColor(_green);
    sf::FloatRect greenRoundBounds = _greenRoundText.getGlobalBounds();
    _greenRoundBg.setPosition(greenRoundBounds.left, greenRoundBounds.top);
    _greenRoundBg.setFillColor(sf::Color::Black);
    _greenRoundBg.setSize((sf::Vector2f){greenRoundBounds.width + 3, greenRoundBounds.height + 3});
}

void Assets::PlayAssets::ammoAssets()
{
    _ammoText.setFont(_font);
    _ammoText.setString("SHOT");
    _ammoText.setCharacterSize(20.f);
    _ammoText.setPosition(260.f, 960.f);
    _ammoText.setFillColor(_blue);
    _ammoBg.setSize((sf::Vector2f){97.f, 60.f});
    _ammoBg.setFillColor(sf::Color::Black);
    _ammoBg.setOutlineThickness(3.f);
    _ammoBg.setOutlineColor(_green);
    _ammoBg.setPosition(250.f, 930.f);
    _bulletSpr.setTexture(_spritesheet);
    _bulletSpr.setTextureRect(sf::IntRect(0, 550, 25, 30));
    for (int i = 0; i < 3; i++) {
        sf::Sprite bullet(_bulletSpr);
        bullet.setPosition(260.f + i * 30.f, 927.f);
        _bullets.push_back(bullet);
    }
}

void Assets::PlayAssets::hitAssets()
{
    _redDuckSpr.setTexture(_spritesheet);
    _redDuckSpr.setTextureRect(sf::IntRect(26, 557, 20, 20));
    _whiteDuckSpr.setTexture(_spritesheet);
    _whiteDuckSpr.setTextureRect(sf::IntRect(53, 557, 20, 20));
    _hitText.setFont(_font);
    _hitText.setString("HIT");
    _hitText.setPosition(1200.f, 947.f);
    _hitText.setFillColor(_green);
    _hitBg.setSize((sf::Vector2f){450.f, 60.f});
    _hitBg.setFillColor(sf::Color::Black);
    _hitBg.setOutlineThickness(3.f);
    _hitBg.setOutlineColor(_green);
    _hitBg.setPosition(1175.f, 930.f);
    for (int i = 0; i < 10; i++) {
        sf::Sprite redDuck(_redDuckSpr);
        sf::Sprite whiteDuck(_whiteDuckSpr);

        redDuck.setPosition(1325.f + i * 30.f, 950.f);
        whiteDuck.setPosition(1325.f + i * 30.f, 950.f);
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
    _scoreText.setCharacterSize(25.f);
    _scoreText.setPosition(1710.f, 935.f);
    _scoreBg.setSize((sf::Vector2f){175.f, 60.f});
    _scoreBg.setFillColor(sf::Color::Black);
    _scoreBg.setOutlineThickness(3.f);
    _scoreBg.setOutlineColor(_green);
    _scoreBg.setPosition(1700.f, 930.f);
}

void Assets::PlayAssets::visualAssets()
{
    _backgroundText.loadFromFile("assets/play/background.png");
    _backgroundSpr.setTexture(_backgroundText);
    resizeSprite(_backgroundSpr, _backgroundText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _backgroundSpr.setPosition(0, 0);
    _spritesheet.loadFromFile("assets/play/spriteSheet.png");
    _grassText.loadFromFile("assets/play/tall_grasses.png");
    _grassSpr.setTexture(_grassText);
    resizeSprite(_grassSpr, _grassText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _grassSpr.setPosition(0, 0);
}

void Assets::PlayAssets::pauseAssets()
{
    _pauseText.loadFromFile("assets/play/pause.png");
    _pauseSpr.setTexture(_pauseText);
    resizeSprite(_pauseSpr, _pauseText, 300, 200);
    centerSprite(_pauseSpr);
}

void Assets::PlayAssets::flyAwayAssets()
{
    _flyAwayText.loadFromFile("assets/play/flyAway.png");
    _flyAwaySpr.setTexture(_flyAwayText);
    resizeSprite(_flyAwaySpr, _flyAwayText, 300, 75);
    _flyAwaySpr.setPosition(0, 100);
    centerSpriteAxisX(_flyAwaySpr);
}

void Assets::PlayAssets::setRound(int round)
{
    _round = round;
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