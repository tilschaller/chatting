#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("Chatting client started\n");

  Display *MainDisplay = XOpenDisplay(0);
  Window RootWindow = XDefaultRootWindow(MainDisplay);

  int DefaultScreen = DefaultScreen(MainDisplay);
  GC Context = XDefaultGC(MainDisplay, DefaultScreen);

  int WindowX = 0;
  int WindowY = 0;
  int WindowWidth = 800;
  int WindowHeight = 600;
  int BorderWidth = 0;
  int WindowDepth = CopyFromParent;
  int WindowClass = CopyFromParent;
  Visual *WindowVisual = CopyFromParent;

  int AttributeValueMask = CWBackPixel | CWEventMask;
  XSetWindowAttributes WindowAttributes = {};
  WindowAttributes.background_pixel = 0xffffffff;
  WindowAttributes.event_mask =
      StructureNotifyMask | KeyPressMask | KeyReleaseMask | ExposureMask;

  Window MainWindow =
      XCreateWindow(MainDisplay, RootWindow, WindowX, WindowY, WindowWidth,
                    WindowHeight, BorderWidth, WindowDepth, WindowClass,
                    WindowVisual, AttributeValueMask, &WindowAttributes);

  XMapWindow(MainDisplay, MainWindow);

  Atom WM_DELETE_WINDOW = XInternAtom(MainDisplay, "WM_DELETE_WINDOW", False);
  if (!XSetWMProtocols(MainDisplay, MainWindow, &WM_DELETE_WINDOW, 1)) {
    printf("Couldn't register WM_DELETE_WINDOW property \n");
  }

  int IsWindowOpen = 1;
  while (IsWindowOpen) {
    XEvent GeneralEvent = {};

    XNextEvent(MainDisplay, &GeneralEvent);

    switch (GeneralEvent.type) {
    case KeyPress:
    case KeyRelease: {
      XKeyPressedEvent *Event = (XKeyPressedEvent *)&GeneralEvent;
      if (Event->keycode == XKeysymToKeycode(MainDisplay, XK_Escape)) {
        IsWindowOpen = 0;
      }
    } break;

    case ClientMessage: {
      XClientMessageEvent *Event = (XClientMessageEvent *)&GeneralEvent;
      if ((Atom)Event->data.l[0] == WM_DELETE_WINDOW) {
        XDestroyWindow(MainDisplay, MainWindow);
        IsWindowOpen = 0;
      }
    } break;
    }
    XClearWindow(MainDisplay, MainWindow);
  }
  return 0;
}
