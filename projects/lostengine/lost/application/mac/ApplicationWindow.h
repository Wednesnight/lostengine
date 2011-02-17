#import <AppKit/NSWindow.h>

namespace lost
{
namespace application
{
    struct Window;
}
}

#if MAC_OS_X_VERSION_MIN_REQUIRED < 1060
@interface ApplicationWindow : NSWindow
#else
@interface ApplicationWindow : NSWindow<NSWindowDelegate>
#endif
{
  lost::application::Window* parent;

  bool leftButtonDown;
  bool rightButtonDown;

  NSUInteger modifierFlags;
}

- (void)mouseEvent: (NSEvent*)event type:(const char*)type;
- (void)setParent: (lost::application::Window*)newParent;

@end
