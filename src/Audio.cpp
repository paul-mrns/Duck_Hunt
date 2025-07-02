/*
** Paul Mourens project
** Duck Hunt cpp
** Audio.cpp
*/

#include "Core.hpp"

void DuckHunt::Audio::createSound(std::string path)
{
    _soundBuffers.emplace_back();
    if (_soundBuffers.back().loadFromFile(path)) {
        sf::Sound sound;
        sound.setBuffer(_soundBuffers.back());
        _sounds.push_back(sound);
    }
}

void DuckHunt::Audio::createMusic(std::string path)
{
    auto music = std::make_unique<sf::Music>();
    
    if (music->openFromFile(path)) {
        music->setLoop(true);
        _musics.push_back(std::move(music));
    }
}

void DuckHunt::Audio::loadSounds()
{
    createSound(SHOT_PATH);
}

void DuckHunt::Audio::loadMusics()
{
    createMusic(START_PATH);
    createMusic(INTRO_PATH);
    createMusic(FLAP_PATH);
    createMusic(FALL_PATH);
    createMusic(CAUGHT_PATH);
}

DuckHunt::Audio::Audio()
{
    loadSounds();
    loadMusics();
}

void DuckHunt::Audio::play_sound(int sound_id)
{
    if (sound_id > 0 && sound_id <= static_cast<int>(_sounds.size()))
        _sounds[sound_id - 1].play();
}

void DuckHunt::Audio::play_music(int music_id, bool repeat)
{
    for (auto& m : _musics)
        m->stop();
    if (music_id > 0 && music_id <= static_cast<int>(_musics.size())) {
        _musics[music_id - 1]->setLoop(repeat);
        _musics[music_id - 1]->play();
    }
}

void DuckHunt::Audio::stop_music(int music_id)
{
    if (music_id > 0 && music_id <= static_cast<int>(_musics.size()))
        _musics[music_id - 1]->stop();
}
