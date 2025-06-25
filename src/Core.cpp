/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "Core.hpp"
#include <unistd.h>

DuckHunt::Core::Core(std::string &username)
{
    _username = username;
    _window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGTH}), "Duck_Hunt.exe");
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
            case sf::Keyboard::Down: _input = arrow_down; break;
            case sf::Keyboard::Up: _input = arrow_up; break;
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
        _input = left_click;
}

void DuckHunt::Core::menuHandler()
{
    if (!_menu)
        _menu = std::make_unique<Menu>(_highScores);
    _menu->handleInput(_input);
    if (_input != quit)
        _input = none;
    _menu->update();
    _menu->draw(_window);
    if (_menu->isGameStartRequested()) {
        _gamemode = _menu->gamemodeChosen();
        _state = play;
    }
}

void DuckHunt::Core::playHandler()
{
   // if (!play)
        //play = std::make_unique<Play>();

}

void DuckHunt::Core::stateHandler()
{
    if (_state == menu)
        menuHandler();
    else if (_state == play)
        playHandler();
}

void DuckHunt::Core::gameLoop()
{
    sf::Event event;

    loadHighScores(HIGHSCORE_FILE);
    while(_window.isOpen()) {
        while(_window.pollEvent(event))
            handleInput(event);
        if (_input == quit)
            break;
        _window.clear(sf::Color::Black);
        stateHandler();
        _window.display();
    }
    //save high score
}