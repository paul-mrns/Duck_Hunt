/*
** Paul Mourens project
** Duck Hunt cpp
** Sounds.hpp
*/

#pragma once
#include <memory>

#define START_MUSIC 001
#define START_PATH "assets/audio/start.mp3"

#define INTRO_MUSIC 002
#define INTRO_PATH "assets/audio/intro.mp3"

#define SHOT_SOUND 001
#define SHOT_PATH "assets/audio/shot.mp3"

namespace DuckHunt
{
    class Audio
    {
    private:
        std::vector<std::unique_ptr<sf::Music>> _musics;
        std::vector<sf::Sound> _sounds;
        std::vector<sf::SoundBuffer> _soundBuffers;
    public:
        Audio();
        ~Audio() = default;

        void loadSounds();
        void createSound(std::string path);
        void loadMusics();
        void createMusic(std::string path);

        void play_sound(int sound_id);
        void play_music(int music_id, bool repeat);

        void stop_music(int music_id);
    };
}
