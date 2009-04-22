#ifndef __MAINRENDERER_H__
#define __MAINRENDERER_H__

#include <string>
#include <GL/GL.h>
#include <SDL/SDL.h>
#include "generator.h"
#include "RenderEnvironment.h"
#include "Effect.h"
#include "RenderStats.h"

class MainRenderer
{
public:
	MainRenderer(RenderEnvironment& renv);
	~MainRenderer();

	void init(bool fullscreen);

	void render(Uint32 deltatick);
	
private:
	void initSurface(bool fullscreen);
	void initAttributes();
	void createSurface(bool fullscreen);

	void drawString(int x, int y, const std::string& text);

	SDL_Surface* screen;	
    RenderEnvironment& renderenv;
    RenderStats stats;
    lost::shared_ptr<Effect> currentEffect;
};

#endif
