#include "MainRenderer.h"
#include <GLUT/glut.h>
#include <SDL/SDL.h>
#include <iostream>
#include <stdexcept>
#include <cmath>


using namespace std;


MainRenderer::MainRenderer(guiro::UserInterface& inUI)
: stats(renderenv), mUI(inUI)
{
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
    
    // Create window
    screen = SDL_SetVideoMode (MainRenderer::windowWidth, MainRenderer::windowHeight, 0, flags);
    if (screen == NULL) {		
		throw runtime_error(string("Couldn't set OpenGL video mode: ")+SDL_GetError());
	}
}

void MainRenderer::render()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  stats.startRender();
  
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, windowWidth-1, 0.0, windowHeight-1);		
  glDisable(GL_DEPTH_TEST);
  
  mUI.draw();
  
//  glColor3f(1.0f, 1.0f, 1.0f);
//  drawString(100, 100, "zorkbark!!");
  glFlush();
  stats.endRender();
  stats.drawStats();
  SDL_GL_SwapBuffers();
}


void MainRenderer::drawString(int x, int y, const string& text)
{
	int len, i;
	glRasterPos2f(x, y);
	len = text.size();
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
	}
}

