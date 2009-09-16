
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "MainRenderer.h"
#include "RenderEnvironment.h"

class Application
{
public:
	Application();
	~Application();
	
	void run();
		
private:

    enum UserEvent
    {
        event_render = 0
    };

	void startUp();
	void runLoop();
	void shutDown();

    void startRenderTimer();
    void stopRenderTimer();
	static Uint32 renderTimerFunc(Uint32 interval, void* param);

	void render(Uint32 deltaTick);

	void reset();

	SDL_TimerID		renderTimerId;
	RenderEnvironment		renderenv;
	lost::shared_ptr<MainRenderer>	renderer;
    Uint32          lasttick;
};

#endif
