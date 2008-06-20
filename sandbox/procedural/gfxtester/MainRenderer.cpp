
#include "MainRenderer.h"
#include <GLUT/glut.h>
#include <SDL/SDL.h>
#include <iostream>
#include <stdexcept>
#include "TimeUtils.h"
#include "GlUtils.h"
#include <sstream>
#include "CircleEffect.h"
#include "CubeEffect.h"
#include "RingsEffect.h"

using namespace std;


MainRenderer::MainRenderer(RenderEnvironment& renv)
: renderenv(renv), stats(renv)
{
    currentEffect.reset(new RingsEffect(renderenv));
}

MainRenderer::~MainRenderer()
{

}

void MainRenderer::init(bool fullscreen)
{
	initSurface(fullscreen);
}

void MainRenderer::initSurface(bool fullscreen)
{
    initAttributes();    
    createSurface(fullscreen);
}

void MainRenderer::initAttributes()
{
    int value;
    value = 16;
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);
    value = 1;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, value);
}

void MainRenderer::createSurface(bool fullscreen)
{
    Uint32 flags = 0;
    
    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;
    
    screen = SDL_SetVideoMode (renderenv.windowWidth, renderenv.windowHeight, 0, flags);
    if (screen == NULL) {		
		throw runtime_error(string("Couldn't set OpenGL video mode:")+SDL_GetError());
	}
}

void MainRenderer::render(Uint32 deltaTick)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    stats.startRender();
	        
    currentEffect->draw(deltaTick);

    stats.endRender();
    stats.drawStats();

	SDL_GL_SwapBuffers();
}



