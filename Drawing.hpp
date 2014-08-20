#ifndef DRAWING_HEADER
#define DRAWING_HEADER

#include "Objects.hpp"
#include "Image.hpp"
#include <GL/gl.h>
#include <cmath>
#include <iostream>

namespace color {

static float BLACK[]  = {0.0, 0.0, 0.0, 1.0};
static float WHITE[]  = {1.0, 1.0, 1.0, 1.0};
static float RED[]    = {0.93, 0.0, 0.0, 1.0};
static float ORANGE[] = {1.0, 0.3, 0.0, 1.0};
static float GREEN[]  = {0.0, 1.0, 0.0, 1.0};
static float PW[]     = {0.0, 0.8, 0.0, 1.0};
static float BLUE[]   = {0.0, 0.6, 0.8, 1.0};
static float BRICK[]   = {0.54, 0.35, 0.0, 1.0};
static float YELLOW[] = {1.0, 1.0, 0.0, 1.0};
static float TRANS[]  = {0.0, 0.0, 0.0, 0.0};

vector<float*> playercolors;

void load_colors() {
     playercolors.push_back(RED);
}

} // end namespace

inline double radtodeg(const double& rad) {
    return rad/M_PI*180;
}
inline double degtorad(const double& deg) {
    return deg*M_PI/180;
}

void draw_circle(float cx, float cy, float r, int num_segments=60) 
{ 
    float theta = 2 * M_PI / num_segments; 
    float c = cosf(theta); // precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1, y = 0; // we start at angle = 0 
    
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5, 0.5);
        glVertex2f(cx, cy);
        for(int ii = 0; ii <= num_segments; ii++) { 
            glTexCoord2f(0.5 + x/2, 0.5 + y/2);
            glVertex2f(x*r + cx, y*r + cy); // output vertex 

            t = x;                      // apply the rotation matrix of unit circle
            x = c * x - s * y;
            y = s * t + c * y;
        } 
    glEnd(); 
}

void draw_rect(int x, int y, int w, int h) {
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(x,   y);   
        glTexCoord2i(1, 0); glVertex2f(x+w, y);   
        glTexCoord2i(1, 1); glVertex2f(x+w, y+h); 
        glTexCoord2i(0, 1); glVertex2f(x,   y+h); 
    glEnd();
}

void draw_line(const Vec& start, const Vec& end, const float width=2) {
    glPushMatrix();
    glTranslatef(start.x, start.y, 0);
    Vec relv = end - start;
    float phi = radtodeg(atan2(relv.y, relv.x));
    glRotatef(phi,0,0,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex2f(0, -width);
        glTexCoord2f(0,0); glVertex2f(0, width);
        float abs = relv.size();
        glTexCoord2f(abs/2/width,0); glVertex2f(abs, width);
        glTexCoord2f(abs/2/width,1); glVertex2f(abs, -width);
    glEnd();
    glPopMatrix();
}

void draw_arrow(const Vec& start, const Vec& end, const float width=2,
                const float head=10, const float phi=30) { // head is head lenght, phi is head offset angle
    draw_line(start, end, width);
    // head line 1
    float c = cos(degtorad(phi)), s = sin(degtorad(phi));
    Vec hl1 = start - end, hl2;
    hl1.normalize();
    hl1 *= head;
    hl2 = hl1;
    hl1.matrix_multiply(c, -s, s, c);
    hl2.matrix_multiply(c, s, -s, c);
    hl1 += end;
    hl2 += end;
    draw_line(end, hl1, width);
    draw_line(end, hl2, width);
    draw_circle(start.x, start.y, width);
    draw_circle(end.x, end.y, width);
    draw_circle(hl1.x, hl1.y, width);
    draw_circle(hl2.x, hl2.y, width);
}

void draw(const Bomb& b, int gs) {
     image::bombs_img[b.pid]->activate();  
     draw_rect(b.x*gs, b.y*gs, gs-2, gs-2);
     image::deactivate();
}

void draw(const PowerUp& pu, int gs) {
     if (pu.bombcounting != 0){
        image::pwrBum_img->activate();  
     }else if (pu.rangecounting != 0){
        image::pwrRange_img->activate();     
     }else if (pu.bombkicking != 0){
        image::pwrKick_img->activate();
     }
     draw_rect(pu.x*gs, pu.y*gs, gs-2, gs-2);
     image::deactivate();
     //glPopMatrix();
}

void draw(const World& w, int sw, int sh, int gs) {
     //glPushMatrix();
     for (int i=0; i< w.h; i++) {
         for (int j=0; j< w.w; j++) {
             glColor4fv(color::WHITE);
             draw_rect(j*gs, i*gs, gs-2, gs-2);
             switch (w.grid[i][j]){ 
                   case 0:
                        image::sand_img->activate();
                        break;
                   case 1:
                        image::sand_img->activate();
                        draw_rect(j*gs, i*gs, gs-2, gs-2);
                        image::breakable_img->activate();
                        break;
                   case 2:
                        image::unbreak_img->activate();
                        break;
                   case 3:
                        glColor4fv(color::WHITE);
                        image::sand_img->activate();
                        break;
             }
             draw_rect(j*gs, i*gs, gs-2, gs-2);
             image::deactivate();
             
         }
     }
     list<PowerUp>::const_iterator it = w.pu.begin();
     while (it != w.pu.end()) {
         draw(*it, gs);
         ++it;
     }
     
     list<Bomb>::const_iterator bit = w.bombs.begin();
     while (bit != w.bombs.end()) {
         draw(*bit, gs);
         ++bit;
     }
}

void draw(const Player& pl, int gs) {
     //glColor4fv(color::playercolors[pl.id]);
     image::player_img[pl.id]->activate();
     draw_rect(pl.x*gs, pl.y*gs, gs-2, gs-2);
     image::deactivate();       
}

void draw(const vector<Player>& pl, int gs) {
     for (int i = 0; i < pl.size(); ++i) {
         if (!pl[i].dead) {
            draw(pl[i], gs);
         }
     }
}

#endif
