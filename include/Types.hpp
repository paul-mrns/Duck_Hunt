/*
** Paul Mourens project
** Duck Hunt cpp
** Types.hpp
*/

#pragma once
#include <string>

namespace DuckHunt
{
    enum game_state {menu, play, click, hit, settings, game_end};
    enum input {left_click, enter, arrow_up, arrow_down, escape, quit, none};

    struct PlayerScore {
        std::string username;
        int score;
    };
}