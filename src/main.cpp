/*
** Paul Mourens project
** Duck Hunt cpp
** main.cpp
*/

#include "Core.hpp"
#include <string.h>

static int displayUsage()
{
    std::cout << "./DuckHunt.exe <USERNAME>" << std::endl;
    std::cout << "USERNAME: Your username will be stored with your high score" << std::endl;
    return 0;
}

static int displayError(std::string errorMessage)
{
    std::cerr << "DuckHunt.exe: " << errorMessage << std::endl;
    displayUsage();
    return ERROR;
}

static std::string strToUppercase(char *string)
{
    std::string username(string, strlen(string));

    for (size_t i = 0; i < username.length(); i++)
        if (username[i] >= 'a' && username[i] <= 'z')
            username[i] -= 32;
    return username;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return displayError("Please enter an username");
    if (strlen(argv[1]) > 6)
        return displayError("Max username length is 6");
    if (strcmp(argv[1], "-h") == 0)
        return displayUsage();

    std::string username = strToUppercase(argv[1]);
    DuckHunt::Core c(username);
}