#include "game.hpp"
//https://www.fesliyanstudios.com/ for background music
Game::Game()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed at SDL_Init()" << SDL_GetError() << endl;
    if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren) < 0) cout << "Failed at SDL_CreateWindowAndRenderer()" << SDL_GetError() << endl;
    SDL_SetWindowTitle(win, "DodgeTheCars");
    TTF_Init();
    running = true;

    //road strip
    s1.setDest(531, -200, 1, 1280);
    s1.setSource(0, 0, 1, 1280);
    s1.setImage("assets/roadstrip.png", ren);
    s2.setDest(627, -200, 1, 1280);
    s2.setSource(0, 0, 1, 1280);
    s2.setImage("assets/roadstrip.png", ren);
    s3.setDest(723, -200, 1, 1280);
    s3.setSource(0, 0, 1, 1280);
    s3.setImage("assets/roadstrip.png", ren);
    s4.setDest(819, -200, 1, 1280);
    s4.setSource(0, 0, 1, 1280);
    s4.setImage("assets/roadstrip.png", ren);

    //tree, flower, lamp post
    num = 0;
    lamp1.setDest(375, -200, 48, 96);
    lamp1.setSource(0, 0, 48, 48);
    lamp1.setImage("assets/lampostL.png", ren);
    lamp2.setDest(912, -200, 48, 96);
    lamp2.setSource(0, 0, 48, 48);
    lamp2.setImage("assets/lampostR.png", ren);
    tree.setDest(970, -800, 96, 96);
    tree.setSource(0, 0, 48, 48);
    tree.setImage("assets/tree.png", ren);
    flower.setDest(340, -750, 20, 20);
    flower.setSource(0, 0, 48, 48);
    flower.setImage("assets/flowers.png", ren);
    

    count = 3;
    srand(time(NULL));
    car.setDest(440, -200, 38*2, 67*2);
    car.setSource(0, 0, 38, 67);
    car.setImage("assets/silver.png", ren);
    car.setSpeed(1);
    car2.setDest(540, -500, 38*2, 67*2);
    car2.setSource(0, 0, 38, 67);
    car2.setImage("assets/orange.png", ren);
    car2.setSpeed(1);
    car3.setDest(640, -820, 38*2, 67*2);
    car3.setSource(0, 0, 38, 67);
    car3.setImage("assets/white.png", ren);
    car3.setSpeed(1);
    car4.setDest(735, -100, 38*2, 67*2);
    car4.setSource(0, 0, 38, 67);
    car4.setImage("assets/red.png", ren);
    car4.setSpeed(1);
    car5.setDest(835, -900, 38*2, 67*2);
    car5.setSource(0, 0, 38, 67);
    car5.setImage("assets/blue.png", ren);
    car5.setSpeed(1);

    font = TTF_OpenFont("assets/arial.ttf", 24);

    loadMap("1.level");
    bgmusic.load("music/FunnyBit.wav");
    effect.load("music/police.wav");
    effect2.load("music/Explosion.wav");
    player.setImage("assets/animated.png", ren);
    player.setDest(639, 866, 38*2, 67*2);
    idol = player.createCycle(1, 38, 67, 1, 0);
    siren = player.createCycle(1, 38, 67, 3, 10);
    player.setCurAnimation(idol);
    mapX = mapY = 0;
    speed = 6;
    bgmusic.play(); //background music
    loop();

}

Game::~Game()
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
}

void Game::loop()
{
    static int lastTime = 0;
    while(running)
    {
        lastFrame = SDL_GetTicks();
        //if(lastFrame >= (lastTime+1000))
        //{
            //lastTime = lastFrame;
            //frameCount = 0;
        //}
        //cout<<mousex<<" "<<mousey<<endl;
    
        render();
        input();
        update();
    }
}

void Game::render()
{
    drawMap();
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = 150;
    rect.h = 85;
    SDL_RenderFillRect(ren, &rect);

    draw(s1);
    draw(s2);
    draw(s3);
    draw(s4);
    draw(lamp1);
    draw(lamp2);
    draw(tree);
    draw(flower);

    start = 800;
    gameScore = (SDL_GetTicks()-start) / 100;
    string score = "Score: " + to_string(gameScore);
    draw(score.c_str(), 20, 30, 255, 255, 255);

    draw(player);

    draw(car);
    draw(car2);
    draw(car3);
    draw(car4);
    draw(car5);

    frameCount++;
    int timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS<(1000/60))
    {
        SDL_Delay((1000/60) - timerFPS);
    }    

    SDL_RenderPresent(ren);
}

void Game::draw(Object a)
{
    SDL_Rect dest = a.getDest();
    SDL_Rect src = a.getSource();
    SDL_RenderCopyEx(ren, a.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void Game::draw(const char* msg, int x, int y, int r, int g, int b)
{
    SDL_Surface* surf;
    SDL_Texture* tex;
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
    SDL_Rect rect;
    surf = TTF_RenderText_Solid(font, msg, color);
    tex = SDL_CreateTextureFromSurface(ren, surf);
    rect.x = x;
    rect.y = y;
    rect.w = surf->w;
    rect.h = surf->h;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(ren, tex, NULL, &rect);
    SDL_DestroyTexture(tex);
}

void Game::input()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            running = false;
            bgmusic.close(music);
            effect.close(music);
            effect2.close(music);
            cout<<"Quiting"<<endl;
        }
        if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == SDLK_ESCAPE) running = false;
            if(e.key.keysym.sym == SDLK_SPACE) 
            {
                if(player.getCurAnimation() == siren)
                {
                    player.setCurAnimation(idol);
                    effect.pause();
                }
                else
                {
                    player.setCurAnimation(siren);
                    effect.play();
                }
            }
            if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {l=1;r=0;}
            if(e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {r=1;l=0;}
            if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {u=1;}
            if(e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {d=1;}
        }
        if(e.type == SDL_KEYUP)
        {
            if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {l=0;}
            if(e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {r=0;}
            if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) u=0;
            if(e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) d=0;
        }
        //SDL_GetMouseState(&mousex, &mousey);
    }
}

void Game::update()
{
    if(l) {player.setDest(player.getDX()-speed, player.getDY());}
    if(r) {player.setDest(player.getDX()+speed, player.getDY());}
    if(u) {player.setDest(player.getDX(), player.getDY()-speed);}
    if(d) {player.setDest(player.getDX(), player.getDY()+speed);}
    player.updateAnimation();

    //road strip on repeat
    s1.setDest(s1.getDX(), s1.getDY()+ count + 1);
    s2.setDest(s2.getDX(), s2.getDY()+ count + 1);
    s3.setDest(s3.getDX(), s3.getDY()+ count + 1);
    s4.setDest(s4.getDX(), s4.getDY()+ count + 1);
    if(s1.getDY() > 10)
    {
        s1.setDest(531, -150);
        s2.setDest(627, -150);
        s3.setDest(723, -150);
        s4.setDest(819, -150);
    }


    //flower, lamp, tree on repeat
    lamp1.setDest(lamp1.getDX(), lamp1.getDY()+ count + 1);
    lamp2.setDest(lamp2.getDX(), lamp2.getDY()+ count + 1);
    if(lamp1.getDY() > 1000)
    {
        lamp1.setDest(375, -200);
        lamp2.setDest(912, -200);
    }
    tree.setDest(tree.getDX(), tree.getDY()+ count + 1);
    flower.setDest(flower.getDX(), flower.getDY()+ count + 1);
    if(tree.getDY() > 1000)
    {
        if(num%2 == 0)
        {
            tree.setDest(275, -200);
            flower.setDest(970, -150);
            num++;
        }
        else
        {
            tree.setDest(970, -200);
            flower.setDest(345, -150);
            num--;
        }
    }

    
    car.setDest(car.getDX(), car.getDY()+car.getSpeed()*(count));
    if(car.getDX() == 440 && car.getDY() > 1000)
    {
        car.setDest(440, -220);
        car.setSpeed(rand() % 4 + 1);
    }
    car2.setDest(car2.getDX(), car2.getDY()+car2.getSpeed()*(count));
    if(car2.getDX() == 540 && car2.getDY() > 1000)
    {
        car2.setDest(540, -220);
        car2.setSpeed(rand() % 5 + 1);
    }
    car3.setDest(car3.getDX(), car3.getDY()+car3.getSpeed()*(count));
    if(car3.getDX() == 640 && car3.getDY() > 1000)
    {
        car3.setDest(640, -220);
        car3.setSpeed(rand() % 5 + 1);
    }
    car4.setDest(car4.getDX(), car4.getDY()+car4.getSpeed()*(count));
    if(car4.getDX() == 735 && car4.getDY() > 1000)
    {
        car4.setDest(735, -220);
        car4.setSpeed(rand() % 5 + 1);
    }
    car5.setDest(car5.getDX(), car5.getDY()+car5.getSpeed()*(count));
    if(car5.getDX() == 835 && car5.getDY() > 1000)
    {
        car5.setDest(835, -220);
        car5.setSpeed(rand() % 5 + 1);
    }


    if(collision(player, car) || collision(player, car2) || collision(player, car3) || collision(player, car4) || collision(player, car5))
    {
        effect2.play();  
        sleep(1); //if you want to listen to explosion, if not commment this out
        cout<<endl<<"GAME OVER!"<<endl<<"Your score is: " << gameScore<<endl; //prints out score
        running = false;
        bgmusic.close(music);
        effect.close(music);
        effect2.close(music);
    } 


    for(int i=0; i<map.size(); i++) // BORDERS
    {
        if(collision(player, map[i])) 
        {
            if(map[i].getId() == 29) //right border
            {
                player.setDest(player.getDX()-speed, player.getDY());
            }

            if(map[i].getId() == 1) //left border
            {
                player.setDest(player.getDX()+speed, player.getDY());
            }

            if(map[i].getId() == 30 ||
                map[i].getId() == 32 ||
                map[i].getId() == 34 ||
                map[i].getId() == 36) //top border
            {
                player.setDest(player.getDX(), player.getDY()+speed);
            }

            if(map[i].getId() == 31 ||
                map[i].getId() == 33 ||
                map[i].getId() == 35 ||
                map[i].getId() == 37) //bottom border
            {
                player.setDest(player.getDX(), player.getDY()-speed);
            }
        }
    }
}

void Game::loadMap(const char* filename)
{
    Object temp;
    temp.setImage("assets/road2.png", ren);
    int current, mx, my, mw, mh;
    ifstream in(filename);
    if(!in.is_open())
    {
        cout<<"Failed to open map file"<<endl;
        return;
    }
    in >> mw;
    in >> mh;
    in >> mx;
    in >> my;
    for(int i = 0; i<mh; i++)
    {
        for(int j =0; j<mw; j++)
        {
            if(in.eof())
            {
                cout<< "Reach end of map file too soon"<<endl;
                return;
            }
            in >> current;
            if(current != 0)
            {
                temp.setSolid(0);
                temp.setSource((current)*16, 0, 16, 16);
                temp.setDest((j*TILE_SIZE)+mx, (i*TILE_SIZE)+my, TILE_SIZE, TILE_SIZE);
                temp.setID(current);
                map.push_back(temp);
            }
        }
    }
    in.close();
}

void Game::drawMap()
{
    for(int i = 0; i < map.size(); i++)
    {
        if(map[i].getDX() >= mapX-TILE_SIZE
        & map[i].getDY() >= mapY-TILE_SIZE
        & map[i].getDX() <= mapX+WIDTH+TILE_SIZE
        & map[i].getDY() <= mapY+HEIGHT+TILE_SIZE)
        {
            draw(map[i]);
        }
    }
}

bool Game::collision(Object a, Object b)
{
    if((a.getDX() < (b.getDX() + b.getDW()-10)) && ((a.getDX() + a.getDW()-10) > b.getDX()) 
  && (a.getDY() < (b.getDY() + b.getDH()-10)) && ((a.getDY() + a.getDH()-10) > b.getDY()))
    {
        return true;
    }
    else
    {
        return false;
    }
}