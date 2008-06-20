#include "Application.h"
#include <SDL/SDL.h>
#include <stdexcept>
#include <iostream>
#include "guiro/Screen.h"
#include "guiro/UserInterface.h"
#include "guiro/ResourceLoader.h"
#include "guiro/Button.h"
#include "guiro/Caption.h"

#include <boost/bind.hpp>

using namespace std;
using namespace boost;
using namespace guiro;


Application::Application()
: mScreen(MainRenderer::windowWidth,
  MainRenderer::windowHeight),
  mUI(mScreen), 
  mResourceLoader(mFontFactory),
  mVanillaFactory(mResourceLoader)
{
    renderer.reset(new MainRenderer(mUI));
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
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) < 0)
	{
        throw runtime_error(string("Couldn't initialize SDL: ")+SDL_GetError());
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    
	renderer->init(false);
    mVanillaFactory.init();
    buildUI();
    startRenderTimer();
    lasttick = SDL_GetTicks();    
}

void Application::clicky(guiro::Entity* inEntity)
{
    cout << "clicked: " << inEntity->getName() << endl;
}

// build UI AFTER OpenGL has been initialised during renderer initialisation or texture creation will EXPLODE!
void Application::buildUI()
{
    // set UI area first or nothign will draw properly, width/height relastive to screen
    mUI.setArea(make_area(dim::xabs(0), dim::yabs(0), dim::wrel(1.0), dim::hrel(1.0)));

/*    boost::shared_ptr<guiro::Font> zigfont = mResourceLoader.loadFont("suigeneris.ttf", 20);
    boost::shared_ptr<guiro::Quad> renderedText = zigfont->render("Cancel", guiro::whiteColor);
    renderedText->setArea(make_area(dim::xcenter(), dim::ycenter(), dim::wabs(renderedText->getTextureWidth()), dim::habs(renderedText->getTextureHeight())));
    
    guiro::Area full = make_area(dim::xabs(0), dim::yabs(0), dim::wrel(1), dim::hrel(1));
    
    guiro::Color maincol = guiro::Color(.4, .4, .4, 1);

    boost::shared_ptr<guiro::Quad> testback(new guiro::Quad());
    testback->setArea(make_area(dim::xabs(0), dim::yabs(0), dim::wrel(1), dim::hrel(1)));
    testback->setColor(guiro::Color(1.0, 0, 0, .8));

    boost::shared_ptr<guiro::View> mainWindowBackview(new guiro::View());
    boost::shared_ptr<guiro::Image> mainWindowBackImage = mVanillaFactory.buildRoundRectangle(mVanillaFactory.grey1);
    mainWindowBackImage->add(renderedText);
    mainWindowBackview->setBack(mainWindowBackImage);
    mainWindowBackview->setArea(full);
        
    boost::shared_ptr<guiro::Caption> caption = mVanillaFactory.buildCaption();
    caption->setText("asdkjlj");
    caption->setArea(make_area(dim::xabs(0), dim::yabs(0), dim::wabs(100), dim::habs(100)));

    boost::shared_ptr<guiro::Caption> caption2 = mVanillaFactory.buildCaption();
    caption2->setText("knuff");
    caption2->setArea(make_area(dim::xalignright(), dim::yaligntop(), dim::wabs(100), dim::habs(100)));

    
    mainWindowBackview->add(caption);
    mainWindowBackview->add(caption2);

    boost::shared_ptr<guiro::Window> mainWindow(new guiro::Window());
    mainWindow->setArea(make_area(dim::xabs(300), dim::yabs(100), dim::wabs(200), dim::habs(200)));
    mainWindow->setName("mainWindow");
    mainWindow->setBack(mainWindowBackview);

    boost::shared_ptr<guiro::Window> testWindow = mVanillaFactory.buildWindow();
    testWindow->setArea(make_area(dim::xabs(30), dim::yabs(30), dim::wabs(200), dim::habs(100)));
    boost::shared_ptr<guiro::View> testWindowContent(new guiro::View());
    testWindowContent->setName("testWindowContent");
    shared_ptr<guiro::Button> okButton = mVanillaFactory.buildButton();
    okButton->setText("Ok");
    okButton->setName("OkButton");
    okButton->setArea(make_area(dim::xalignleft(), dim::ycenter(), dim::wabs(50), dim::habs(20)));
    testWindowContent->add(okButton);

    shared_ptr<guiro::Button> cancelButton = mVanillaFactory.buildButton();
    cancelButton->setText("Cancel");
    cancelButton->setName("CancelButton");
    cancelButton->setArea(make_area(dim::xalignright(), dim::ycenter(), dim::wabs(50), dim::habs(20)));
    testWindowContent->add(cancelButton);
*/
//    testWindow->setFront(testWindowContent);
    boost::shared_ptr<guiro::Window> testWindow = mVanillaFactory.buildWindow();
    testWindow->setArea(make_area(dim::xabs(30), dim::yabs(30), dim::wabs(200), dim::habs(100)));
    testWindow->setName("testWindow");
    testWindow->setTitle("testWindow gnöGg");
    
    boost::shared_ptr<guiro::Window> testWindow2 = mVanillaFactory.buildWindow();
    testWindow2->setArea(make_area(dim::xabs(60), dim::yabs(60), dim::wabs(200), dim::habs(100)));
    testWindow2->setName("testWindow2");
    testWindow2->setTitle("testWindow2");
    
    
//    mUI.add(mainWindow);    
    mUI.add(testWindow);
    mUI.add(testWindow2);
        
//    (*testWindow)["frontView"]["OkButton"].as<Button>().buttonClicked.connect(boost::bind(&Application::clicky, this, _1));
//    (*testWindow)["frontView"]["CancelButton"].as<Button>().buttonClicked.connect(boost::bind(&Application::clicky, this, _1));
}

void Application::shutDown()
{
    stopRenderTimer();
	SDL_Quit();
}

void Application::render(Uint32 deltaTick)
{
	renderer->render();
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
			case SDL_MOUSEMOTION:
                mUI.mouseMotion(guiro::Point(event.motion.x, (mScreen.h-1)-event.motion.y));
				break;		
            case SDL_MOUSEBUTTONDOWN:
                mUI.mouseButtonDown(guiro::EventProcessor::button1, guiro::Point(event.button.x, (mScreen.h-1)-event.button.y));
                break;
            case SDL_MOUSEBUTTONUP:
                mUI.mouseButtonUp(guiro::EventProcessor::button1, guiro::Point(event.button.x, (mScreen.h-1)-event.button.y));
                break;
			case SDL_KEYDOWN:
				cout << "key" << endl;
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:done=true;break;
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


