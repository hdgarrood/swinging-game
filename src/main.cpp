#include <string>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <Box2D/Box2D.h>

#include "SDL_DebugDraw.h"
#include "Constants.h"

SDL_Surface *load_image(std::string filename)
{
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load(filename.c_str());

    if (loadedImage != NULL)
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *dest)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, NULL, dest, &offset);
}

Uint32 ColourBlack(SDL_Surface *screen)
{
    return SDL_MapRGBA(screen->format, 0x00, 0x00, 0x00, 0xFF);
}

Uint32 ColourWhite(SDL_Surface *screen)
{
    return SDL_MapRGBA(screen->format, 0xFF, 0xFF, 0xFF, 0xFF);
}

b2World CreateWorld()
{
    b2Vec2 gravity(0.0f, 9.81f);
    b2World m_world(gravity);

    // create the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    //groundBodyDef.position.Set(0.0f, 20.0f);

    b2Body *groundBody = m_world.CreateBody(&groundBodyDef);

    b2Vec2 groundLeft(0.0f, 30.0f);
    b2Vec2 groundRight(64.0f, 48.0f);
    b2EdgeShape groundShape;
    groundShape.Set(groundLeft, groundRight);
    groundBody->CreateFixture(&groundShape, 0.0f);

    // create a circle
    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(1.0f, 1.0f);
    circleBodyDef.type = b2_dynamicBody;

    b2Body *circleBody = m_world.CreateBody(&circleBodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 1.0f;
    circleBody->CreateFixture(&circleShape, 1.0f);

    return m_world;
}

void debug(std::string message)
{
    puts(message.c_str());
}

int main(int argc, char *args[])
{
    debug("About to SDL_Init");
    SDL_Init(SDL_INIT_EVERYTHING);

    debug("About to create screen surface");
    SDL_Surface *screen = NULL;
    screen = SDL_SetVideoMode(SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SCREEN_BPP,
            SDL_SWSURFACE);

    debug("About to create world");
    b2World world = CreateWorld();
    SDL_DebugDraw debugDraw(screen, ColourWhite(screen));

    world.SetDebugDraw(&debugDraw);

    for (int i=0; i<300; i++)
    {
        debug("About to step");
        world.Step(STEP_TIME, POSITION_ITERATIONS, VELOCITY_ITERATIONS);
        world.ClearForces();

        debug("About to draw world");
        SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_FillRect(screen, &rect, ColourBlack(screen));
        world.DrawDebugData();

        SDL_Flip(screen);
        debug("About to delay");
        SDL_Delay(16);
    }

    SDL_FreeSurface(screen);
    SDL_Quit();
    return 0;
}
