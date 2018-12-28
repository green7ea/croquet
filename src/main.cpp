#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES

#include "audio.hpp"
#include "game.hpp"

#include <GL/gl.h>
#include <SDL.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

static void sdldie(const char *msg)
{
    fprintf(stderr, "%s \n\t%s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

static void checkSDLError(int line)
{
    const char *error = SDL_GetError();
    if (error[0] != '\0')
    {
        fprintf(stderr, "SDL Error at line %i\n\t%s\n", line, error);
        SDL_ClearError();
    }
}

static void draw(SDL_Surface *surface)
{
    Game &game = Game::getGame();

    game.draw(surface);

    SDL_GL_SwapBuffers();
}

static void reshape(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

static void event()
{
    SDL_Event event;

    Game &game = Game::getGame();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            game.running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                game.running = false;
            }
            else
            {
                game.keypress(event.key.keysym.sym);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            game.mouseDown(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            game.mouseUp(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_MOUSEMOTION:
            game.mouseMove(event.motion.x, event.motion.y, event.motion.xrel,
                           event.motion.yrel);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        sdldie("Couldn't initialize SDL");
    }

    SDL_Surface *display = SDL_SetVideoMode(
        800, 600, 24, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);
    if (!display)
    {
        sdldie("Couldn't create a window");
    }
    checkSDLError(__LINE__);

    reshape(800, 600);

    Game &game = Game::getGame();
    while (game.running)
    {
        event();
        game.update();
        draw(display);
    }

    Game::destroyGame();
    Audio::destroyAudio();
    SDL_Quit();

    return 0;
}
