#include <string>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <Box2D/Box2D.h>

const int SCREEN_WIDTH                  = 640;
const int SCREEN_HEIGHT                 = 480;
const int SCREEN_BPP                    = 32;

const float32 STEP_TIME                 = 1.0f / 60.0f;
const int32 POSITION_ITERATIONS         = 2;
const int32 VELOCITY_ITERATIONS         = 6;
const float PPM                         = 10.0f;

struct Vec2
{
    int x;
    int y;
};

// apply scaling to a b2Vec2
Vec2 Scaleb2Vec2(b2Vec2 vec)
{
    Vec2 retval = {(int)floor(vec.x * PPM), (int)floor(vec.y * PPM)};
    return retval;
}

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
    b2Vec2 gravity(0.0f, 10.0f);
    b2World m_world(gravity);

    // create the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    //groundBodyDef.position.Set(0.0f, 20.0f);

    b2Body *groundBody = m_world.CreateBody(&groundBodyDef);

    b2Vec2 groundLeft(0.0f, 20.0f);
    b2Vec2 groundRight(30.0f, 48.0f);
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

void PutPixel(SDL_Surface *surface, int x, int y, Uint32 colour)
{
    Uint32 *pixels = (Uint32*)surface->pixels;
    pixels[(surface->w * y) + x] = colour;
}

void DebugDrawCircle(b2Body *body, b2CircleShape *circle, SDL_Surface *screen)
{
    Vec2 position = Scaleb2Vec2(body->GetPosition());

    DebugDrawEdge
}

void DebugDrawPolygon(b2PolygonShape *polygon, SDL_Surface *screen)
{
    // TODO write this properly

}

// lifted from:
// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_with_Integer_Arithmetic
void DebugDrawEdge(b2EdgeShape *edge, SDL_Surface *screen)
{
    debug("printing an edge");
    Vec2 vec1 = Scaleb2Vec2(edge->m_vertex1);
    Vec2 vec2 = Scaleb2Vec2(edge->m_vertex2);

    const bool steep = (fabs(vec2.y - vec1.y) > fabs(vec2.x - vec1.x));
    if (steep)
    {
        std::swap(vec1.x, vec1.y);
        std::swap(vec2.x, vec2.y);
    }

    if (vec1.x > vec2.x)
    {
        std::swap(vec1.x, vec2.x);
        std::swap(vec1.y, vec2.y);
    }

    Uint32 colour = ColourWhite(screen);

    int dx = vec2.x - vec1.x;
    int dy = vec2.y - vec1.y;

    int D = 2*dy - dx;

    PutPixel(screen, vec1.x, vec1.y, colour);
    int y = vec1.y;

    for (int x = vec1.x + 1; x != vec2.x; x++)
    {
        if (D > 0)
        {
            y++;
            PutPixel(screen, x, y, colour);
            D += ((2 * dy) - (2 * dx));
        }
        else
        {
            PutPixel(screen, x, y, colour);
            D += 2 * dy;
        }
    }
}

void DebugDrawBody(b2Body *body, SDL_Surface *screen)
{
    for (b2Fixture *fixture = body->GetFixtureList();
    fixture != NULL;
    fixture = fixture->GetNext())
    {
        switch (fixture->GetType())
        {
            case (b2Shape::e_circle):
            {
                b2CircleShape *circle = (b2CircleShape*)fixture->GetShape();
                DebugDrawCircle(body, circle, screen);
                break;
            }
            /* case b2Shape::e_polygon: */
            /* { */
            /*     b2PolygonShape *polygon = (b2PolygonShape*)fixture->GetShape(); */
            /*     DebugDrawPolygon(polygon, screen); */
            /*     break; */
            /* } */
            case b2Shape::e_edge:
            {
                b2EdgeShape *edge = (b2EdgeShape*)fixture->GetShape();
                DebugDrawEdge(edge, screen);
                break;
            }
            default:
                debug("Failed to draw unknown shape type.");
                break;
        }
    }
}

// Draw every body in the world onto the surface
void DebugDrawWorld(b2World *world, SDL_Surface *screen)
{
    if (SDL_MUSTLOCK(screen))
    {
        SDL_LockSurface(screen);
    }

    debug("about to get body list");
    b2Body *body = world->GetBodyList();
    debug("got the world's body list");
    while (body != NULL)
    {
        debug("about to draw a body");
        DebugDrawBody(body, screen);
        debug("drawn a body");
        body = body->GetNext();
    }

    if (SDL_MUSTLOCK(screen))
    {
        SDL_UnlockSurface(screen);
    }
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

    for (int i=0; i<300; i++)
    {
        debug("About to step");
        world.Step(STEP_TIME, POSITION_ITERATIONS, VELOCITY_ITERATIONS);
        world.ClearForces();

        debug("About to draw world");
        SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_FillRect(screen, &rect, ColourBlack(screen));
        DebugDrawWorld(&world, screen);

        SDL_Flip(screen);
        debug("About to delay");
        SDL_Delay(16);
    }

    SDL_FreeSurface(screen);
    SDL_Quit();
    return 0;
}
