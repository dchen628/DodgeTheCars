#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
using namespace std;
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <fstream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include <unistd.h>

#include "audio.hpp"
#include "object.hpp"
#include "Entity.hpp"

#define WIDTH 1280
#define HEIGHT 950
#define TILE_SIZE 16

class Game{
public:
    Game();
    ~Game();
    void loop();
    void update();
    void input();
    void render();
    void draw(Object o);
    void draw(const char* msg, int x, int y, int r, int g, int b);
    void loadMap(const char* filenname);
    void drawMap();
    bool collision(Object a, Object b);

private:
    SDL_Renderer* ren;
    SDL_Window* win;
    TTF_Font *font;
    bool running;
    int count, num;
    int frameCount, timerFPS, lastFrame;
    int gameScore, start;
    Object s1, s2, s3, s4, tree, lamp1, lamp2, flower;
    Entity car, car2, car3, car4, car5;
    int mousex, mousey;
    Audio effect, bgmusic, effect2;
    Entity player;
    int idol, siren;
    vector<Object> map;
    int mapX, mapY;
    int speed;
    bool l, r, u, d;
    Mix_Chunk *music;
};


#endif