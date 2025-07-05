/*
** Paul Mourens project
** Duck Hunt cpp
** Types.hpp
*/

#pragma once

namespace DuckHunt
{
    enum gameState {menu, play};
    enum input {leftClick, enter, arrowUp, arrowDown, escape, quit, none};
    enum gamemode {oneDuck, twoDucks, clayShooting, unknownGamemode};

    struct PlayerScore {
        std::string username;
        int score;
    };
}