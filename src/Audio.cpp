/*
** Paul Mourens project
** Duck Hunt cpp
** Audio.cpp
*/

#include "Core.hpp"

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
    _shotSoundBuffer.loadFromFile(SHOT_PATH);
    _shotSound.setBuffer(_shotSoundBuffer);
    _pauseSoundBuffer.loadFromFile(PAUSE_PATH);
    _pauseSound.setBuffer(_pauseSoundBuffer);
}

void DuckHunt::Audio::loadMusics()
{
    createMusic(START_PATH);
    createMusic(INTRO_PATH);
    createMusic(FLAP_PATH);
    createMusic(FALL_PATH);
    createMusic(CAUGHT_PATH);
    createMusic(LAUGH_PATH);
}

DuckHunt::Audio::Audio()
{
    loadSounds();
    loadMusics();
}

void DuckHunt::Audio::playSound(int sound_id)
{
    if (sound_id == SHOT_SOUND)
        _shotSound.play();
    if (sound_id == PAUSE_SOUND)
        _pauseSound.play();
}

void DuckHunt::Audio::playMusic(int music_id, bool repeat)
{
    for (auto& m : _musics)
        m->stop();
    if (music_id > 0 && music_id <= static_cast<int>(_musics.size())) {
        _musics[music_id - 1]->setLoop(repeat);
        _musics[music_id - 1]->play();
        _currentMusicIndex = music_id -1;
    }
}

void DuckHunt::Audio::stopMusic(int music_id)
{
    if (music_id > 0 && music_id <= static_cast<int>(_musics.size()))
        _musics[music_id - 1]->stop();
}

void DuckHunt::Audio::pauseCurrentMusic()
{
    if (_currentMusicIndex >= 0 && _currentMusicIndex < static_cast<int>(_musics.size())) {
        if (_musics[_currentMusicIndex]->getStatus() == sf::SoundSource::Playing)
            _musics[_currentMusicIndex]->pause();
    }
}

void DuckHunt::Audio::resumeCurrentMusic()
{
    if (_currentMusicIndex >= 0 && _currentMusicIndex < static_cast<int>(_musics.size())) {
        if (_musics[_currentMusicIndex]->getStatus() == sf::SoundSource::Paused)
            _musics[_currentMusicIndex]->play();
    }
}
