/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "Core.hpp"

Assets::PlayAssets::PlayAssets(std::vector<int> values)
{
    if (values.size() != 5)
        return;
    _round = values[0];
    _ammo = values[1];
    _score = values[2];
    _hitScore = values[3];
    _perfectRoundBonus = values[4];
    _font.loadFromFile(PLAY_FONT_PATH);
    visualAssets();
    roundAssets();
    ammoAssets();
    hitAssets();
    scoreAssets();
    pauseAssets();
    flyAwayAssets();
    hitScoreAssets();
    perfectRoundAssets();
    gameOverAssets();
}

void Assets::PlayAssets::visualAssets()
{
    _backgroundText.loadFromFile("assets/play/background.png");
    _backgroundSpr.setTexture(_backgroundText);
    resizeSprite(_backgroundSpr, _backgroundText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _backgroundSpr.setPosition(0, 0);
    _spritesheet.loadFromFile("assets/play/spritesheet.png");
    _grassText.loadFromFile("assets/play/tall_grasses.png");
    _grassSpr.setTexture(_grassText);
    resizeSprite(_grassSpr, _grassText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _grassSpr.setPosition(0, 0);
    _flyAwayBackgroundText.loadFromFile("assets/play/fly_away_background.png");
    _flyAwayBackgroundSpr.setTexture(_flyAwayBackgroundText);
    resizeSprite(_flyAwayBackgroundSpr, _flyAwayBackgroundText, WINDOW_WIDTH, WINDOW_HEIGHT);
    _flyAwayBackgroundSpr.setPosition(0, 0);

}

void Assets::PlayAssets::roundAssets()
{
    _roundText.setFont(_font);
    _roundText.setString("ROUND\n\n  " + std::to_string(_round));
    sf::FloatRect roundBounds = _roundText.getLocalBounds();
    _roundText.setPosition((WINDOW_WIDTH - roundBounds.width) / 2.f - roundBounds.left, 200.f);
    _roundText.setFillColor(sf::Color::White);
    _roundBgText.loadFromFile("assets/play/text_background.png");
    resizeSprite(_roundBgSpr, _roundBgText, 200, 135);
    _roundBgSpr.setPosition((1920 / 2 - 100), 175);
    //centerSpriteAxisX(_roundBgSpr);
    _roundBgSpr.setTexture(_roundBgText);
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
    _ammoText.setPosition(260.f, 964.f);
    _ammoText.setFillColor(_blue);
    _ammoBg.setSize((sf::Vector2f){97.f, 60.f});
    _ammoBg.setFillColor(sf::Color::Black);
    _ammoBg.setOutlineThickness(3.f);
    _ammoBg.setOutlineColor(_green);
    _ammoBg.setPosition(250.f, 930.f);
    _bulletSpr.setTexture(_spritesheet);
    _bulletSpr.setTextureRect(sf::IntRect(0, 557, 20, 20));
    for (int i = 0; i < 3; i++) {
        sf::Sprite bullet(_bulletSpr);
        bullet.setPosition(260.f + i * 30.f, 935.f);
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
    _hitText.setPosition(1230.f, 940.f);
    _hitText.setCharacterSize(22);
    _hitText.setFillColor(_green);
    _hitBg.setSize((sf::Vector2f){415.f, 60.f});
    _hitBg.setFillColor(sf::Color::Black);
    _hitBg.setOutlineThickness(3.f);
    _hitBg.setOutlineColor(_green);
    _hitBg.setPosition(1215.f, 930.f);
    _requiredBar.setFont(_font);
    _requiredBar.setFillColor(_blue);
    _requiredBar.setCharacterSize(15);
    _requiredBar.setLetterSpacing(0.01f);
    _requiredBar.setString("|||||||||||||||||");
    setRound(_round);
    _requiredBar.setPosition(1318.f, 968.f);
    for (int i = 0; i < 10; i++) {
        sf::Sprite redDuck(_redDuckSpr);
        sf::Sprite whiteDuck(_whiteDuckSpr);

        redDuck.setPosition(1325.f + i * 30.f, 938.f);
        whiteDuck.setPosition(1325.f + i * 30.f, 938.f);
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

void Assets::PlayAssets::pauseAssets()
{
    _pauseText.loadFromFile("assets/play/pause.png");
    _pauseSpr.setTexture(_pauseText);
    resizeSprite(_pauseSpr, _pauseText, 300, 200);
    _pauseSpr.setPosition(0.f, 350.f);
    centerSpriteAxisX(_pauseSpr);
}

void Assets::PlayAssets::flyAwayAssets()
{
    _flyAwayText.loadFromFile("assets/play/fly_away.png");
    _flyAwaySpr.setTexture(_flyAwayText);
    resizeSprite(_flyAwaySpr, _flyAwayText, 300, 75);
    _flyAwaySpr.setPosition(0, 400);
    centerSpriteAxisX(_flyAwaySpr);
}

void Assets::PlayAssets::hitScoreAssets()
{
    _hitScoreText.setFont(_font);
    _hitScoreText.setString(std::to_string(_hitScore));
    _hitScoreText.setCharacterSize(15);
    _hitScoreText.setFillColor(sf::Color::White);
}

void Assets::PlayAssets::perfectRoundAssets()
{
    _perfectRoundText.setFont(_font);
    _perfectRoundText.setString("PERFECT!!");
    _perfectRoundText.setCharacterSize(30);
    _perfectRoundText.setPosition(0, 165);
    centerTextAxisX(_perfectRoundText);
    _perfectRoundScoreText.setFont(_font);
    _perfectRoundScoreText.setString(std::to_string(_perfectRoundBonus));
    _perfectRoundScoreText.setCharacterSize(30);
    _perfectRoundScoreText.setPosition(0, 250);
    centerTextAxisX(_perfectRoundScoreText);
}

void Assets::PlayAssets::gameOverAssets()
{
    _textBgText.loadFromFile("assets/play/text_background.png");
    _textBgSpr.setTexture(_textBgText);
    resizeSprite(_textBgSpr, _textBgText, 300, 150);
    _textBgSpr.setPosition(0, 150);
    centerSpriteAxisX(_textBgSpr);
    _gameoverText.setFont(_font);
    _gameoverText.setString("GAME OVER");
    _gameoverText.setCharacterSize(25.f);
    _gameoverText.setPosition(0, 215);
    centerTextAxisX(_gameoverText);
}

void Assets::PlayAssets::setRound(int round)
{
    _round = round;
    if (_round <= 0)
        return;
    if (_round >= 1 && round <= 10)
        _requiredBar.setString(std::string(6 * 3, '|'));
    else if (_round >= 11 && _round <= 12)
        _requiredBar.setString(std::string(7 * 3, '|'));
    else if (_round >= 13 && _round <= 14)
        _requiredBar.setString(std::string(8 * 3, '|'));
    else if (_round >= 15 && _round <= 19)
        _requiredBar.setString(std::string(9 * 3, '|'));
    else 
        _requiredBar.setString(std::string(10 * 3, '|'));       
}

void Assets::PlayAssets::setAmmo(int ammo)
{
    _ammo = ammo;
}

void Assets::PlayAssets::setScore(int score)
{
    _score = score;
    _scoreText.setString(alignString(_score) + "\n SCORE");
}

void Assets::PlayAssets::setHitScore(int hitScore, sf::Vector2f pos)
{
    _hitScore = hitScore;
    _hitScoreText.setString(std::to_string(_hitScore));
    _hitScoreText.setPosition(pos);
}
