/*
** Paul Mourens project
** Duck Hunt cpp
** Types.hpp
*/

#pragma once

namespace DuckHunt
{
    enum game_state {menu, play, game_end};
    enum input {left_click, enter, arrow_up, arrow_down, escape, quit, none};
    enum gamemode {oneDuck, twoDucks, clayShooting, unknownGamemode};

    struct PlayerScore {
        std::string username;
        int score;
    };
}