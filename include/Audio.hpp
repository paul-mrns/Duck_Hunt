/*
** Paul Mourens project
** Duck Hunt cpp
** Sounds.hpp
*/

#pragma once

#define START_MUSIC 001
#define START_PATH "assets/audio/start.mp3"

#define INTRO_MUSIC 002
#define INTRO_PATH "assets/audio/intro.mp3"

#define FLAP_MUSIC 003
#define FLAP_PATH "assets/audio/flap.mp3"

#define FALL_MUSIC 004
#define FALL_PATH "assets/audio/fall.mp3"

#define CAUGHT_MUSIC 005
#define CAUGHT_PATH "assets/audio/caught.mp3"

#define LAUGH_MUSIC 006
#define LAUGH_PATH "assets/audio/laugh.mp3"

#define SHOT_SOUND 001
#define SHOT_PATH "assets/audio/shot.mp3"

#define PAUSE_SOUND 002
#define PAUSE_PATH "assets/audio/pause.mp3"

namespace DuckHunt
{
    class Audio
    {
        private:
            std::vector<std::unique_ptr<sf::Music>> _musics;
            sf::Sound _pauseSound;
            sf::SoundBuffer _pauseSoundBuffer;
            sf::Sound _shotSound;
            sf::SoundBuffer _shotSoundBuffer;

            void loadSounds();
            void loadMusics();
            void createMusic(std::string path);
        public:
            Audio();
            ~Audio() = default;

            int _currentMusicIndex = 001;

            //play
            void playSound(int sound_id);
            void playMusic(int music_id, bool repeat);
            void resumeCurrentMusic();

            //stop
            void stopMusic(int music_id);
            void pauseCurrentMusic();

    };
}
