#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Audio
{
public:
    Audio();
    ~Audio();
    void load(const char* filename);
    void play();
    void pause();
    void close(Mix_Chunk *sound);
private:
    Mix_Chunk *sound;
};


#endif