#include "audio.hpp"

Audio::Audio()
{
}
Audio::~Audio()
{
}

void Audio::load(const char* filename)
{
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    sound = Mix_LoadWAV(filename);
}

void Audio::play()
{
    Mix_PlayChannel(-1, sound, -1);
}

void Audio::pause()
{
    Mix_HaltChannel(1);
}

void Audio::close(Mix_Chunk *sound)
{
    Mix_FreeChunk(sound);
    sound = NULL;
}