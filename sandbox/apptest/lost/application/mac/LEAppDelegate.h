#include <AppKit/NSApplication.h>

namespace lost{ namespace application{ struct Application; }}

#if MAC_OS_X_VERSION_MIN_REQUIRED < 1060
@interface LEAppDelegate : NSObject
#else
@interface LEAppDelegate : NSObject<NSApplicationDelegate>
#endif
{
  lost::application::Application* application;
}

- (void)setApplication:(lost::application::Application*)application;

@end
