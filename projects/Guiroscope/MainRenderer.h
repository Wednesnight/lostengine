#ifndef __MAINRENDERER_H__
#define __MAINRENDERER_H__

#include <string>
#include <GL/GL.h>
#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <SDL/SDL_ttf.h>
#include "lostbase/RenderStats.h"

#include "guiro/UserInterface.h"

class MainRenderer
{
public:
	static const unsigned long windowWidth = 640;
	static const unsigned long windowHeight = 480;
	
	MainRenderer(guiro::UserInterface& inUI);
	~MainRenderer();

	void init(bool fullscreen);

	void render();
	

private:
	void initSurface(bool fullscreen);
	void initAttributes();
	void createSurface(bool fullscreen);

	void drawString(int x, int y, const std::string& text);

	SDL_Surface* screen;
    RenderEnvironment renderenv;
    RenderStats stats;    	
    
    guiro::UserInterface& mUI;
};

#endif
