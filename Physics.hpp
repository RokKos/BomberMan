#ifndef PHYSICS_HEADER
#define PHYSICS HEADER

#include "Objects.hpp"
#include "Functions.hpp"
#include "Drawing.hpp"

#include <iostream>

using std::endl;
using std::cout;

//template <typename T>
//void printlist(T a, T b) {
//     for (; a != b; ++a) {
//         cout << a->x << ' ' << a->y << ", ";         
//     }     
//     cout << endl;
//}



void kill_pl(int x, int y, vector<Player>& players){
    vector<Player>::iterator it = players.begin();
    while (it != players.end()) {
        if (it->x == x && it->y == y){
            it->die();
        }
        it++;
    }
}

void explode(World& w, list<Bomb>::iterator it, vector<Player>& p) {
     int power = p[it->pid].power;
     int x = it->x;
     int y = it->y;
     p[it->pid].activebombs--;
     
     //cout << 
     
     for (int i = 0; i < power; ++i) {
         if (x+i >= w.w) break;
         if (w.grid[y][x+i] == 2) break;
         if (w.grid[y][x+i] == 1) {
              w.grid[y][x+i] = 0;
              w.drop_pu(x+i, y);
               break;
         }
         kill_pl(x+i,y, p);
     }
     for (int i = 0; i < power; ++i) {
         if (x-i < 0) break;
         if (w.grid[y][x-i] == 2) break;
         if (w.grid[y][x-i] == 1) {
              w.grid[y][x-i] = 0;
              w.drop_pu(x-i, y);
              break;
         }
         kill_pl(x-i,y,p);
     }
     for (int i = 0; i < power; ++i) {
         if (y+i >= w.h) break;
         if (w.grid[y+i][x] == 2) break;
         if (w.grid[y+i][x] == 1) {
              w.set(y+i, x, 0);
              w.drop_pu(x, y+i);
              break;
         }
         kill_pl(x,y+i,p);
     }
     for (int i = 0; i < power; ++i) {
         if (y-i < 0) break;
         if (w.grid[y-i][x] == 2) break;
         if (w.grid[y-i][x] == 1) {
              w.grid[y-i][x] = 0; 
              w.drop_pu(x, y-i);
              break;
         }
         kill_pl(x,y-i,p);
     }
     
//     printlist(w.bombs.begin(), w.bombs.end());
     w.bombs.erase(it);
//     printlist(w.bombs.begin(), w.bombs.end());
     w.set(y,x,0);
     // neki novga kar svet za eksplozijo
}

// explosions and shit
void check_explosions(World& w, vector<Player>& p){
     list<Bomb>::iterator it = w.bombs.begin();
     while (it != w.bombs.end()) {
         if (it->t <= 0) {
             list<Bomb>::iterator m = it;
             it++;
             explode(w, m, p);
         } else {
             it->t--;
             it++;
         }
     }
}
void pick_powerup(World& w, vector<Player>& p){
     list<PowerUp>::iterator it= w.pu.begin();
     for (; it!= w.pu.end(); ++it){
         for (int i=0; i<p.size(); ++i){
             if (it->x==p[i].x && it->y==p[i].y){
                 p[i] += *it;
                 it = --w.pu.erase(it);
                 break;
                 }
             }
         }
     }        
     
#endif
