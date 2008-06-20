
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "MainRenderer.h"
#include <boost/shared_ptr.hpp>
#include "guiro/ResourceLoader.h"
#include "guiro/FontFactory.h"
#include "guiro/VanillaFactory.h"
#include "guiro/Entity.h"

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

    void buildUI();

    void startRenderTimer();
    void stopRenderTimer();
	static Uint32 renderTimerFunc(Uint32 interval, void* param);

	void render(Uint32 deltaTick);
    void clicky(guiro::Entity* inEntity);


	boost::shared_ptr<MainRenderer>	renderer;
   	SDL_TimerID		renderTimerId;
    Uint32          lasttick;
    guiro::Screen   mScreen;
    guiro::FontFactory mFontFactory;
    guiro::ResourceLoader mResourceLoader;
    guiro::VanillaFactory mVanillaFactory;
    guiro::UserInterface mUI;
};

#endif
