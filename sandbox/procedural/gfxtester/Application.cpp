#include "Application.h"
#include <SDL/SDL.h>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace boost;

Application::Application()
{
	renderer.reset(new MainRenderer(renderenv));
}

Application::~Application()
{

}

void Application::run()
{
	startUp();
	runLoop();
	shutDown();
}

void Application::startRenderTimer()
{
    unsigned long renderSpeedMsec = 20;
	renderTimerId = SDL_AddTimer(renderSpeedMsec, renderTimerFunc, NULL);
}

void Application::stopRenderTimer()
{
	SDL_RemoveTimer(renderTimerId);
}

Uint32 Application::renderTimerFunc(Uint32 interval, void* param)
{
    SDL_Event event;
    
    event.type = SDL_USEREVENT;
    event.user.code = event_render;
    event.user.data1 = 0;
    event.user.data2 = 0;
    
    SDL_PushEvent(&event);

	return interval;
}

void Application::startUp()
{
	// Init SDL video subsystem
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) < 0)
	{
        throw runtime_error(string("Couldn't initialize SDL: ")+SDL_GetError());
	}

	// FIXME: this repeats ALL keypreses, we might only want this for left/right movement
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    
	renderer->init(true);
	reset();
    startRenderTimer();
    lasttick = SDL_GetTicks();
}

void Application::reset()
{
}

void Application::shutDown()
{
    stopRenderTimer();
	SDL_Quit();
}

void Application::render(Uint32 deltaTick)
{
	renderer->render(deltaTick);
}

void Application::runLoop()
{
    SDL_Event event;
    bool done = false;
	    
	while ((!done) &&  SDL_WaitEvent (&event) )
	{
		switch (event.type)
		{
   			case SDL_USEREVENT:
				switch(event.user.code)
				{
					case event_render:
                        Uint32 currentTick = SDL_GetTicks();
                        Uint32 deltatick = currentTick - lasttick;
                        render(deltatick);
                        lasttick = currentTick;
                        break;
                }
                break;
			case SDL_KEYDOWN:
				cout << "key" << endl;
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:done=true;break;
					case SDLK_SPACE:break;
					case SDLK_UP:break;
					case SDLK_DOWN:break;
					case SDLK_LEFT:break;
					case SDLK_RIGHT:break;
					case SDLK_q:break;
					case SDLK_w:break;
					case SDLK_a:break;
					case SDLK_s:break;
					default:break;
				}
				break;
			case SDL_QUIT:
				done = true;
				break;
			default:
				break;
		}
	}
}



