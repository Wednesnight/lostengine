#ifndef LOST_APPLICATION_IPHONE_LOSTAPPLICATIONHELPERS_H
#define LOST_APPLICATION_IPHONE_LOSTAPPLICATIONHELPERS_H

#include "lost/forward/boost"
#include "lost/event/Event.h"

void* lostApplicationHelpers_linkHelper();

void lostApplicationHelpers_runAppController(); // starts up UIKit app controller
void lostApplicationHelpers_swapBuffers(); // tells GlView to swap buffers
void lostApplicationHelpers_quitAppController(); // shuts down UIKit app controller

// these are the old contents of Application::run, subdivided and wrapped up in simple C functions
// that operate on the global application instance.
// This is necessary since lost::application::Application has to perform the startup, but once the controller
// is in charge it won't return until shutdown, so the custom app controller has to take over the role
// of the application. Since this isn't possible due to the initial structure of the lost engine, 
// the controller can call these functions in order to perform the intended lifecycle operations.

// one otehr reason to factor out even simple parts of code into seperate C functions is the fact
// that including luabind in objective-c files produces namespace clashes. Therefore, if the Objective-C 
// application controller needs to send an event to the lost::Application, it can't access the appInstance
// directly, but must do so via the sendEvent function. 

void lostApplicationHelpers_preinit();
void lostApplicationHelpers_resize(unsigned short width, unsigned short height);
void lostApplicationHelpers_dispatchEvent(boost::shared_ptr<lost::event::Event> event);
void lostApplicationHelpers_init();
void lostApplicationHelpers_run();
void lostApplicationHelpers_quit();

#endif