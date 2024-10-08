#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/glx.h>

struct Glob {
  Display *disp;
  Window win;
  GLXContext gl_context;
};

void create_gl_context(struct Glob *glob) {
  int screen_id = DefaultScreen(glob->disp);

  int attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

  XVisualInfo *visual_info = glXChooseVisual(glob->disp, screen_id, attribs);
  if (!visual_info) {
    fprintf(stderr, "Couldnt find suitable Opengl visual\n");
  }

  glob->gl_context = glXCreateContext(glob->disp, visual_info, NULL, GL_TRUE);
  if (!glob->gl_context) {
    fprintf(stderr, "Could not create OpenGL Context\n");
  }
}

int main(int argc, char *argv[]) {
  printf("Chatting client started\n");

  struct Glob *glob = malloc(sizeof(struct Glob));

  glob->disp = XOpenDisplay(0);
  if (!glob->disp) {
    fprintf(stderr, "Failed to connect to X display\n");
  }

  glob->win = XCreateSimpleWindow(glob->disp, XDefaultRootWindow(glob->disp), 0,
                                  0, 800, 500, 0, 0, 0);
  if (!glob->win) {
    fprintf(stderr, "Failed to create X window\n");
  }

  XSelectInput(glob->disp, glob->win, ExposureMask);
  XMapWindow(glob->disp, glob->win);

  Atom atom_delete_window = XInternAtom(glob->disp, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(glob->disp, glob->win, &atom_delete_window, 1);

  XFlush(glob->disp);

  create_gl_context(glob);
  glXMakeCurrent(glob->disp, glob->win, glob->gl_context);

  XEvent ev;
  while (1) {
    XNextEvent(glob->disp, &ev);

    if (ev.type == ClientMessage) {
      if (ev.xclient.data.l[0] == atom_delete_window) {
        break;
      }
    }
    if (ev.type == Expose) {
      glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glXSwapBuffers(glob->disp, glob->win);
    }
  }

  glXDestroyContext(glob->disp, glob->gl_context);
  XDestroyWindow(glob->disp, glob->win);
  XCloseDisplay(glob->disp);

  free(glob);

  return 0;
}
