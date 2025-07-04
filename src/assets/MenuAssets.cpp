/*
** Paul Mourens project
** Duck Hunt cpp
** Core.cpp
*/

#include "assets/MenuAssets.hpp"

Assets::MenuAssets::MenuAssets(const std::vector<DuckHunt::PlayerScore> &highscores)
{
    logoAssets();
    textAssets(highscores);
}

void Assets::MenuAssets::logoAssets()
{
    _logoText.loadFromFile("./assets/menu/logo.jpg");
    _logoSpr.setTexture(_logoText);
    resizeSprite(_logoSpr, _logoText, LOGO_WIDTH, LOGO_HEIGHT);
    _logoSpr.setPosition((WINDOW_WIDTH - LOGO_WIDTH) / 2.f, 0);
}

void Assets::MenuAssets::createScoreboard()
{
    sf::Color orange(255, 128, 13);

    _scoreboard.setPosition(600.f, 100.f);
    _scoreboard.setSize((sf::Vector2f){720.f, 780.f});
    _scoreboard.setFillColor(sf::Color::Black);
    _scoreboard.setOutlineThickness(5.f);
    _scoreboard.setOutlineColor(orange);

    _highscoreText.setFont(_font);
    _highscoreText.setString("HIGH SCORES");
    _highscoreText.setFillColor(orange);
    _highscoreText.setCharacterSize(40);
    _highscoreText.setOutlineThickness(2.f);
    _highscoreText.setOutlineColor(sf::Color::Red);
    sf::FloatRect highscoreBounds = _highscoreText.getLocalBounds();
    _highscoreText.setPosition((WINDOW_WIDTH - highscoreBounds.width) / 2.f - highscoreBounds.left, 150.f);
}

void Assets::MenuAssets::createHeader()
{
    sf::Text header;

    header.setFont(_font);
    header.setString("RANK   SCORE   NAME");
    header.setFillColor(sf::Color::White);
    header.setOutlineThickness(2.f);
    header.setOutlineColor(sf::Color::Magenta);
    header.setPosition(650.f, 275.f);
    _highScores.push_back(header);
}

static sf::Color colorForRanks(int rank)
{
    sf::Color color;

    if (rank == 1)
        color = {255, 215, 0}; //gold
    else if (rank == 2)
        color = {192, 192, 192}; //silver
    else if (rank == 3)
        color = {205, 127, 50}; //bronze
    else
        color = sf::Color::White;
    return color;
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

void Assets::MenuAssets::createPodium(const std::vector<DuckHunt::PlayerScore> &highscores)
{
    std::vector<std::string> ranks = {"1st", "2nd", "3rd"};
    std::string line;
    int y = 350;

    for (size_t i = 0; i < highscores.size() && i < 3; i++) {
        sf::Text scoreline;
        scoreline.setFont(_font);
        scoreline.setFillColor(colorForRanks(i + 1));
        scoreline.setPosition(650.f, y);
        y += 50;
        line = alignString(highscores[i].score);
        scoreline.setString(ranks[i] + "    " + line + "  " + highscores[i].username);
        _highScores.push_back(scoreline);
    }
}

void Assets::MenuAssets::createScores(const std::vector<DuckHunt::PlayerScore> &highscores)
{
    createHeader();
    if (highscores.size() == 1 && highscores[0].score == 0 && highscores[0].username == "")
        return;
    createPodium(highscores);
    int y = 500;
    for (size_t i = 3; i < highscores.size() && i < 10; i++) {
        sf::Text scoreline;
        std::string line;
        std::string rank;
        scoreline.setFont(_font);
        scoreline.setFillColor(colorForRanks(i + 1));
        scoreline.setPosition(650.f, y);
        y += 50;
        rank = std::to_string(i + 1) + "th";
        line = alignString(highscores[i].score);
        if (i == 9)
            scoreline.setString(rank + "   " + line + "  " + highscores[i].username);
        else
            scoreline.setString(rank + "    " + line + "  " + highscores[i].username);
        _highScores.push_back(scoreline);
    }
}

void Assets::MenuAssets::textAssets(const std::vector<DuckHunt::PlayerScore> &highscores)
{
    _font.loadFromFile(MENU_FONT_PATH);

    //gamemodes
    sf::Text game0;
    game0.setString("GAME A   1 DUCK");
    game0.setFont(_font);
    game0.setPosition(720.f, 560.f);
    _games.push_back(game0);

    sf::Text game1;
    game1.setString("GAME B   2 DUCKS");
    game1.setFont(_font);
    game1.setPosition(720.f, 610.f);
    _games.push_back(game1);

    sf::Text game2;
    game2.setString("GAME C   CLAY SHOOTING");
    game2.setFont(_font);
    game2.setPosition(720.f, 660.f);
    _games.push_back(game2);

    _scores.setString("CLICK TO SEE SCORES");
    _scores.setFont(_font);
    sf::FloatRect scoresBounds = _scores.getLocalBounds();
    _scores.setPosition((WINDOW_WIDTH - scoresBounds.width) / 2.f - scoresBounds.left, 750.f);

    //top score display
    std::string score = std::to_string(highscores[0].score);
    _topScore.setString("TOP SCORE = " + score + " (" + highscores[0].username + ")");
    _topScore.setFont(_font);
    sf::FloatRect scoreBounds = _topScore.getLocalBounds();
    _topScore.setPosition((WINDOW_WIDTH - scoreBounds.width) / 2.f - scoreBounds.left, 800.f);
    _topScore.setFillColor(sf::Color::Green);

    //credits
    _credits.setString("@2025 PAUL MOURENS CO.,LTD.");
    _credits.setFont(_font);
    _credits.setCharacterSize(40);
    sf::FloatRect creditsBounds = _credits.getLocalBounds();
    _credits.setPosition((WINDOW_WIDTH - creditsBounds.width) / 2.f - creditsBounds.left, 900.f);
    _credits.setFillColor(sf::Color::White);

    //scoreboard
    createScoreboard();
    createScores(highscores);
}

