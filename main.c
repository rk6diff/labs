//
// Created by astronaut on 11.05.19.
//
#include <stdio.h>
#include <X11/Xutil.h>
#include "pulse_circle.h"

int main(int argc, char* argv[]) {
	XPiAr2 circle;
	Display* dpy = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(dpy);
	int screen = DefaultScreen(dpy);
	GC gc[2];
	for (int i = 0; i < 2; ++i) {
		gc[i] = XCreateGC(dpy, root, 0, 0);
	}

	unsigned long tone = WhitePixel(dpy, screen); /* = 0xFFFFFF; */
	XSetForeground(dpy, gc[0], tone);
	XFontStruct* font_ptr = XLoadQueryFont(dpy, "9x15");
	if (font_ptr != NULL) {
		XSetFont(dpy, gc[0], font_ptr->fid);
	}

	XSetWindowAttributes attr; /* window attributes structure */
	XGCValues gval;			   /* GC structure */
	XSizeHints hint;		   /* Geometry WM hints */
	if (argc < 2) { argv[1] = "16x8"; }

	unsigned int width = 0;
	unsigned int height = 0;
	for (;;) {
		if (max_size(&circle, argv[1]) == 0) {
			max_size(&circle, argv[1] = "16x8");
		}
		width = (unsigned int) decent(&circle);
		height = (unsigned int) reset(&circle);
		if (height < DisplayHeight(dpy, screen)){
			break;
		}
		argv[1] = "16x8";
	}

	printf("Now: %s %s (RxN)\n", argv[0], argv[1]);
	fflush(stdout);
	unsigned long amask = (CWOverrideRedirect | CWBackPixel);
	XGetGCValues(dpy, gc[1], GCBackground, &gval);
	attr.background_pixel = gval.background; /* = 0x0 */
	attr.override_redirect = False;
	Window window = XCreateWindow(dpy, root, 0, 0, width, height, 1, CopyFromParent, InputOutput,
						CopyFromParent, amask, &attr);
	hint.flags = (PMinSize | PMaxSize);
	hint.min_width = hint.max_width = width;
	hint.min_height = hint.max_height = height;
	XSetNormalHints(dpy, window, &hint);
	XStoreName(dpy, window, "spiral");
	Atom wdw = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, window, &wdw, 1);
	XMapWindow(dpy, window);

	unsigned long emask = (ExposureMask | KeyPressMask |
						   FocusChangeMask | VisibilityChangeMask | ControlMask);
	XEvent event;				/* graphic event structure */
	int freeze = 0;				/* window visibility stop state */
	int g = 0;					/* GC index */

	XSelectInput(dpy, window, emask);
	int multi = 1;
	int is_inc = true;
	while (multi != 0) { /* Async dispatch event with multic ground */
		event.type = 0;
		XCheckWindowEvent(dpy, window, emask, &event);
		switch (event.type) {
			case Expose:
				redraw(&event, gc[0], &circle);
				break;
			case VisibilityNotify:
				freeze = overlap(&event);
				break;
			case FocusIn:
				rep5355(dpy, AutoRepeatModeOff);
				break;
			case FocusOut:
				rep5355(dpy, AutoRepeatModeOn);
				break;
			case KeyPress: {
              get_event_code(&event, &multi);
              break;
			}
		}

        /* Freeze display */
		if (multi < 0 || freeze < 0) { continue; }

        delay_func(50);
        XClearWindow(dpy, window);
        if (circle.radius >= circle.r_max) { is_inc = false; }

        if (is_inc == false &&
            (circle.radius <= circle.radius_min)) {
          is_inc = true;
        }

        twist(dpy, window, gc[g], &circle, &is_inc);
	}
	rep5355(dpy, AutoRepeatModeOn);
	XDestroyWindow(dpy, window);
	XCloseDisplay(dpy);
	return 0;
}
