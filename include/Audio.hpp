/*
** Paul Mourens project
** Duck Hunt cpp
** Sounds.hpp
*/

#pragma once

#define START_MUSIC 1
#define START_PATH "assets/audio/start.mp3"

#define INTRO_MUSIC 2
#define INTRO_PATH "assets/audio/intro.mp3"

#define FLAP_MUSIC 3
#define FLAP_PATH "assets/audio/flap.mp3"

#define FALL_MUSIC 4
#define FALL_PATH "assets/audio/fall.mp3"

#define CAUGHT_MUSIC 5
#define CAUGHT_PATH "assets/audio/caught.mp3"

#define LAUGH_MUSIC 6
#define LAUGH_PATH "assets/audio/laugh.mp3"

#define GAMEOVER_MUSIC 7
#define GAMEOVER_PATH "assets/audio/gameover.mp3"

#define ROUND_END_MUSIC 8
#define ROUND_END_PATH "assets/audio/round_end.mp3"

#define PERFECT_ROUND_MUSIC 9
#define PERFECT_ROUND_PATH "assets/audio/perfect_round.mp3"

#define SHOT_SOUND 1
#define SHOT_PATH "assets/audio/shot.mp3"

#define PAUSE_SOUND 2
#define PAUSE_PATH "assets/audio/pause.mp3"

#define POINTS_SOUND 3
#define POINTS_PATH "assets/audio/points.mp3"

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
            sf::Sound _pointsSound;
            sf::SoundBuffer _pointsSoundBuffer;

            void loadSounds();
            void loadMusics();
            void createMusic(std::string path);
        public:
            Audio();
            ~Audio() = default;

            int _currentMusicIndex = 001;

            //play
            void playSound(int soundId);
            void playMusic(int musicId, bool repeat);
            void resumeCurrentMusic();

            //stop
            void stopMusic(int musicId);
            void pauseCurrentMusic();

    };
}
