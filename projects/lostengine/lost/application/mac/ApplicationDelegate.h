#include <AppKit/NSApplication.h>

namespace lost{ namespace application{ struct Application; }}

#if MAC_OS_X_VERSION_MIN_REQUIRED < 1060
@interface ApplicationDelegate : NSObject
#else
@interface ApplicationDelegate : NSObject<NSApplicationDelegate>
#endif
{
  lost::application::Application* parent;
}

- (void)setParent: (lost::application::Application*)newParent;

@end
