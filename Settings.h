#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>

bool init(int width, int height) {

    // SDL settings

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    atexit(SDL_Quit);

    SDL_WM_SetCaption("Igrica", "Igrica");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"),NULL);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
     
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,     32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);
     
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8);
     
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_HWSURFACE);
    if (!screen)
    {
        printf("Unable to set %ix%i video: %s\n", width, height, SDL_GetError());
        return false;
    }

    SDL_EnableUNICODE( SDL_ENABLE );
    SDL_EnableKeyRepeat( 250, SDL_DEFAULT_REPEAT_INTERVAL);  

    // GL settings

    glViewport(0, 0, screen->w, screen->h);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLfloat)screen->w, (GLfloat)screen->h, 0.0, 1.0, -1.0); /* l r t b n f */
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); //Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
    glLoadIdentity();

    return true;
}

#endif
