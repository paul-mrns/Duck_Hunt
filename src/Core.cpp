/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "Core.hpp"

DuckHunt::Core::Core(std::string &username)
{
    _username = username;
    _window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Duck_Hunt.exe");
    _window.setFramerateLimit(60);
    _window.setKeyRepeatEnabled(false);
    _state = menu;
    gameLoop();
}

void DuckHunt::Core::handleInput(sf::Event event)
{
    if (_input != none)
        return;
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Down: _input = arrowDown; break;
            case sf::Keyboard::Up: _input = arrowUp; break;
            case sf::Keyboard::Enter: _input = enter; break;
            case sf::Keyboard::Escape: _input = escape; break;
            case sf::Keyboard::C:
                if (event.key.control)
                    _input = quit;
                break;
            default: break;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
        _input = leftClick;
}

void DuckHunt::Core::menuHandler()
{
    if (!_menu) {
        _menu = std::make_unique<Menu>(_highScores, _audio);
        _audio.playMusic(START_MUSIC, false);
    }
    _menu->handleInput(_input, sf::Mouse::getPosition(_window));
    if (_input != quit)
        _input = none;
    _menu->update();
    _menu->draw(_window);
    if (_menu->isGameStartRequested()) {
        _gamemode = _menu->gamemodeChosen();
        _state = play;
        _menu.reset();
        return;
    }
}

void DuckHunt::Core::classicHandler()
{
   if (!_classic) {
        _classic = std::make_unique<Classic>(_score, _audio, _round, _gamemode + 1);
        _audio.playMusic(INTRO_MUSIC, false);
   }
    _classic->handleInput(_input, sf::Mouse::getPosition(_window));
    if (_input != quit)
        _input = none;
    _classic->update();
    _classic->draw(_window);
    if (_classic->isGameOver()) {
        _state = menu;
        _classic.reset();
        saveScore();
        _score = 0;
        return;
    }
    if (_classic->isNewRoundStart()) {
        _classic.reset();
        _round++;
        return;
    }
}

void DuckHunt::Core::stateHandler()
{
    if (_state == menu)
        menuHandler();
    else if (_state == play) {
        if (_gamemode == oneDuck || _gamemode == twoDucks)
            classicHandler();
        else
            std::cout << "Clay Shooting is unavailable at the moment\n";
    }
}

void DuckHunt::Core::gameLoop()
{
    sf::Event event;

    loadHighScores();
    while(_window.isOpen()) {
        while(_window.pollEvent(event))
            handleInput(event);
        if (_input == quit)
            break;
        _window.clear(sf::Color::Black);
        stateHandler();
        _window.display();
    }
}