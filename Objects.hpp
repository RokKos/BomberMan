#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <cstdlib>

using std::vector;
using std::list;
using std::cout;
using std::endl;
using std::ostream;

struct Vec {
    double x, y;
    Vec() {};
    Vec(double x_, double y_) : x(x_), y(y_) {};
    Vec operator+(const Vec& v) const {
        return Vec(x+v.x, y+v.y);
    }
    Vec& operator+=(const Vec& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec operator-(const Vec& v) const {
        return Vec(x-v.x, y-v.y);
    }
    Vec& operator-=(const Vec& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec operator*(const double t) const {
        return Vec(x*t, y*t);
    }
    Vec& operator*=(const double t) {
        x *= t; 
        y *= t;
        return *this;
    }
    double operator*(const Vec& v) {
        return x*v.x + y*v.y;
    }
    double projection(const Vec& v) {
        return *this*v/size();
    }
    double norm() const {
        return x*x+y*y;
    }
    double size() const {
        return sqrt(norm());
    }
    double abs() const {
        return size();
    }
    void normalize() { // normalizes vector, i.e, after this, it has length 1
        double s = size();
        x /= s;
        y /= s;
    }
    void matrix_multiply(double a11, double a12, double a21, double a22) {
        double t = x;
        x = a11 * t + a12 * y;
        y = a21 * t + a22 * y;
    }
    bool operator==(const Vec& v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vec& v) const { return x != v.x || y != v.y; }
//    friend std::ostream& operator<<(st::ostream& o, const Vec& v);
};
std::ostream& operator<<(std::ostream& o, const Vec& v) {
    o << '(' << v.x << ',' << v.y << ')'; return o;
}

double d(const Vec& a, const Vec& b) {
    return (a-b).size();
}

struct Bomb {
       int pid, x, y, t, d;
       Bomb(int x_, int y_, int id_, int d_, int t_) : x(x_), y(y_), pid(id_), t(t_), d(d_) {};
};

struct PowerUp{
       int bombcounting, rangecounting, bombkicking, x,y, tip;
       PowerUp(int x_,int y_, int tip_ ): x(x_), y(y_), tip(tip_) {
            bombcounting = 0; rangecounting = 0; bombkicking = false;
            switch(tip) {
                case 0: bombcounting = 1; break;
                case 1: rangecounting = 1; break;
                case 2: bombkicking = true; break;
            }          
       };
};

struct World {
    vector<vector<int> > grid;
    list<Bomb> bombs;
    list<PowerUp> pu;
    int w, h;
    World(int w_, int h_) : w(w_), h(h_){};
    void make_map(int w, int h) {
        grid.resize(h, vector<int>(w, 0));
    }
    void set(int i,int j,int s){
        grid[i][j]=s;
    }
    bool is_free(int x,int y){
        return grid[y][x] == 0;
    }
    void drop_pu(int x, int y) {
         if (rand() % 5 == 0) {
            int a = rand() % 3;
            PowerUp pup (x, y, a);
            pu.push_back(pup);    
         }       
    }
         
    bool check_limits(int x, int y) {
        return x >= 0 && y >= 0 && x < w && y < h;
    }
    friend ostream& operator << (ostream& os, const World& w);
};
ostream& operator << (ostream& os, const World& w) {
    for (int i = 0; i < w.grid.size(); ++i) {
        for (int j = 0; j < w.grid[i].size(); ++j) {
            os << w.grid[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}

struct Player{  
       int x,y,id, power, bombcount, activebombs, kicking;
       bool dead;
       World* w;
       const static int bomb_timeout = 15; // frames
       Player(int x_, int y_, int id_, World* w_) : x(x_), y(y_), id(id_), w(w_),
                       power(3), bombcount(1), activebombs(0), dead(false), kicking(false) {};
       void left(){ 
            if (!dead && w->is_free(x-1, y)) {
                 x-=1;
            }
            if (!dead && kicking && w->grid[y][x-1] == 3) {
                kickBomb(x,y,-1,0);
                x--;
            }
       }
       void right(){ 
            if (!dead && w->is_free(x+1, y)) {
                 x+=1;
            }
            if (!dead && kicking && w->grid[y][x+1] == 3) {
                kickBomb(x,y,1,0);
                x++;
            }
       }
        void down(){ 
            if (!dead && w->is_free(x, y+1)) {
                 y+=1;
            }
            if (!dead && kicking && w->grid[y+1][x] == 3) {
                kickBomb(x,y,0,1);
                y++;
            }
       }
        void up(){ 
            if (!dead && w->is_free(x, y-1)) {
                 y-=1;
            }
            if (!dead && kicking && w->grid[y-1][x] == 3) {
                kickBomb(x,y,0,-1);
                y--;
            }
       }
       void die() { 
            dead = true;
            x = -10;
            y = -10;
       }
       void drop() {
            if (bombcount > activebombs && !dead){
                activebombs++;                      
                w->bombs.push_back(Bomb(x, y, id, power,  bomb_timeout));
                w->set(y, x, 3);
            }
       }
      Player& operator +=(const PowerUp & up){
             power += up.rangecounting;
             bombcount += up.bombcounting;
             kicking |= up.bombkicking;
             return *this;
      } 
    
    void kickBomb(int x, int y, int u, int v) {
         if (w->check_limits(x+u, y+v) && w->grid[y+v][x+u] == 3) {                  
            int i = 2;
            while(w->check_limits(x+u*i, y+v*i) && w->grid[y+v*i][x+u*i] == 0){
                ++i;
            }
            i--;
            w->grid[y+v*i][x+u*i] = 3;
            w->grid[y+v][x+u] = 0;
            list<Bomb>::iterator bit = w->bombs.begin();
            while (bit != w->bombs.end()) {
                if( bit->x == x+u && bit->y == y+v){
                    bit->x = x+i*u;
                    bit->y = y+i*v;
                }
                ++bit;
            }
        }     
    }       
      
};

#endif
