#include "Settings.h"
#include "Objects.hpp"
#include "Functions.hpp"
#include "Physics.hpp"
#include "Drawing.hpp"

#include <iostream>
#include <sstream>  
#include <string> 
#include <vector>
#include <cstdio>
#include <cassert>
#include <ctime> 
#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>

#include <windows.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 750
#define GRID_SIZE 50

using std::vector;
using std::cout;

int main(int argc, char* argv[]) {
    
    start:
    
    srand (time(NULL));
    
    assert(init(SCREEN_WIDTH, SCREEN_HEIGHT));
    // init successful

    image::load_images();
    color::load_colors();
    
    World world(SCREEN_WIDTH/GRID_SIZE, SCREEN_HEIGHT/GRID_SIZE);
//    int lvl=rand() % 5;
//    string Result;          
//    std::ostringstream convert;   
//    convert << lvl;      
//    Result = convert.str();
    createLvl(SCREEN_WIDTH/GRID_SIZE,SCREEN_HEIGHT/GRID_SIZE,world);
  //  LoadLevel("levels/level."+Result+".txt", world);
    
    Player player(1,1,0,&world);
    Player player2(16,1,1,&world);
    Player player3(1,13,2,&world);
    Player player4(16,13,3,&world);    
    vector<Player> players;
    players.push_back(player);
    players.push_back(player2);
    players.push_back(player3);
    players.push_back(player4);

    bool done = false;
    while(!done) {
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        done = true;
                    }
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        players[0].left();
                    }
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        players[0].right();
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        players[0].up();
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        players[0].down();
                    }
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        players[0].drop();
                    }
                    if (event.key.keysym.sym == SDLK_KP8) {
                        players[1].up();
                    }
                    if (event.key.keysym.sym == SDLK_KP5) {
                        players[1].drop();
                    }
                    if (event.key.keysym.sym == SDLK_KP2) {
                        players[1].down();
                    }
                    if (event.key.keysym.sym == SDLK_KP4) {
                        players[1].left();
                    }
                    if (event.key.keysym.sym == SDLK_KP6) {
                        players[1].right();
                    }
                    if (event.key.keysym.sym == SDLK_w) {
                        players[2].up();
                    }
                    if (event.key.keysym.sym == SDLK_e) {
                        players[2].drop();
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        players[2].down();
                    }
                    if (event.key.keysym.sym == SDLK_a) {
                        players[2].left();
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        players[2].right();
                    }
                    if (event.key.keysym.sym == SDLK_i) {
                        players[3].up();
                    }
                    if (event.key.keysym.sym == SDLK_o) {
                        players[3].drop();
                    }
                    if (event.key.keysym.sym == SDLK_k) {
                        players[3].down();
                    }
                    if (event.key.keysym.sym == SDLK_j) {
                        players[3].left();
                    }
                    if (event.key.keysym.sym == SDLK_l) {
                        players[3].right();
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_MOUSEMOTION:
                    break;

            }
                    //cout << world << endl;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        int t = 0;
        for (int i = 0; i < 4; i++) {
            if (players[i].dead==true) t++;
        }         
        if (t >= 3) goto start;

//        cout << "here" << endl;
        
        // Physics here
        check_explosions(world, players);
        pick_powerup(world, players);
        
        // Drawing here

        draw(world, SCREEN_WIDTH, SCREEN_HEIGHT, GRID_SIZE);
        draw(players, GRID_SIZE);
        
        SDL_GL_SwapBuffers();
        //Sleep(10); //(needs windows.h)
    }

    SDL_Quit();

    return 0;
}

