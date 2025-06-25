/*
** Paul Mourens project
** Duck Hunt cpp
** Menu.cpp
*/

#include "gameStates/Menu.hpp"
#include "assets/MenuAssets.hpp"

DuckHunt::Menu::Menu(const std::vector<PlayerScore>& highScores)
    : _selectedOption(0), _startGame(false), _highScores(highScores)
{
    _assets = std::make_unique<Assets::MenuAssets>(highScores);
}

void DuckHunt::Menu::drawScores(sf::RenderWindow& window)
{
    window.draw(_assets->_scoreboard);
    window.draw(_assets->_highscoreText);
    for (size_t i = 0; i < _assets->_highScores.size(); i++)
        window.draw(_assets->_highScores[i]);
}

void DuckHunt::Menu::draw(sf::RenderWindow &window)
{
    window.draw(_assets->_logoSpr);
    window.draw(_assets->_games[0]);
    window.draw(_assets->_games[1]);
    window.draw(_assets->_games[2]);
    window.draw(_assets->_scores);
    window.draw(_assets->_topScore);
    window.draw(_assets->_credits);
    if (_scoreboardActive)
        drawScores(window);
}

bool DuckHunt::Menu::isGameStartRequested() const
{
    return _startGame;
}

DuckHunt::gamemode DuckHunt::Menu::gamemodeChosen() const
{
    return _gamemode;
}

void DuckHunt::Menu::updateSelection()
{
    sf::Color orange(255, 128, 13);

    for (int i = 0; i < 3; ++i) {
        _assets->_games[i].setFillColor(i == _selectedOption ? sf::Color::Red : orange);
    }
    _assets->_scores.setFillColor(_selectedOption == 3 ? sf::Color::Red : sf::Color::White);
}

DuckHunt::gamemode DuckHunt::Menu::getGamemode(int selection)
{
    if (selection == 0)
        return oneDuck;
    else if (selection == 1)
        return twoDucks;
    else if (selection == 2)
        return clayShooting;
    return unknownGamemode;
}

void DuckHunt::Menu::handleInput(input &in)
{
    if (in == enter) {
        if (_selectedOption == 3)
            _scoreboardActive = true;
        else {
            _gamemode = getGamemode(_selectedOption);
            _startGame = true;
        }
    } else if (in == arrow_down)
        _selectedOption = (_selectedOption == 3) ? 0 : _selectedOption + 1;
    else if (in == arrow_up)
        _selectedOption = (_selectedOption == 0) ? 3 : _selectedOption - 1;
    else if (in == escape)
        if (_scoreboardActive)
            _scoreboardActive = false;
        else
            in = quit;
    else
        return;
}

void DuckHunt::Menu::update()
{
    updateSelection();
}
