/* 2 centred spiral main function */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include "lasth.h"

/* Main function */

int main(int argc, char* argv[]) {
	Display* dpy;              /* Graphic Display */
	Window win;                /* programm main window */
	GC gc[2];                  /* Graphic Context */
	int scr;                   /* screen number = 0 */
	XPiAr2 r;                  /* spiral structure */
	Colormap cmap;                 /* color paletter */
	XColor rgb, exact;

	 /* Display Block */
	unsigned long tone;       /* Light drawing tone */
	XFontStruct* fnptr;       /* font structure address to 9x15 */
	dpy = XOpenDisplay(NULL);
	scr = DefaultScreen(dpy);
	win = DefaultRootWindow(dpy);
	scr = DefaultScreen(dpy);
	cmap = DefaultColormap(dpy, scr);
	if(argc<2)
		argv[1]="400x400";
	if(argc<3)
		argv[2]="white";
	if(XLookupColor(dpy, cmap, argv[2], &rgb, &exact) == 0)
		memcpy(&rgb, &exact, sizeof(XColor));
	XAllocColor(dpy, cmap, &rgb);
	gc[0] = XCreateGC(dpy, win, 0, 0);
	XSetForeground(dpy, gc[0], rgb.pixel);

	gc[1] = XCreateGC(dpy, win, 0, 0);

	if((fnptr = XLoadQueryFont(dpy, "9x15")) != NULL)
		XSetFont(dpy, gc[0], fnptr->fid);


/* Window block */
	unsigned int w=0, h=0;             /* main window width & height */
	XSetWindowAttributes attr; /* window attributes structure */
	XGCValues gval;            /* GC structure */
	unsigned long amask;       /* window attributes mask */
	Window root=win;           /* Display root window */
	XSizeHints hint;           /* Geometry WM hints */
	Atom wdw[1];               /* WM delete window atom */

	while(1) { 
		if(maxisize(&r, argv[1], &w, &h) == 0)
			maxisize(&r, argv[1]="400x400", &w, &h);
		if(h < DisplayHeight(dpy, scr))
			break;
		argv[1] = "400x400";
	} /* if */

	printf("Now: %s %s (XxY), color: %s \n", argv[0], argv[1], argv[2]); fflush(stdout);
	amask = (CWOverrideRedirect | CWBackPixel);
	XGetGCValues(dpy, gc[1], GCBackground, &gval);
	attr.background_pixel = gval.background; /* = 0x0 */
	attr.override_redirect = False;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 1, CopyFromParent,
                   InputOutput, CopyFromParent, amask, &attr);
	hint.flags = (PMinSize | PMaxSize);
	hint.min_width = hint.max_width = w; 
	hint.min_height = hint.max_height = h;
	XSetNormalHints(dpy, win, &hint);
	XStoreName(dpy, win, "pulse circle");
	wdw[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, wdw, 1);
	XMapWindow(dpy, win);


	unsigned long emask;       /* window event mask */
	XEvent event;              /* graphic event structure */
	int freeze=0;              /* window visibility stop state */ 
	unsigned delay=0;    /* multi delay period = 2^rate */
	int multi=1;         /* multi code */
	int g=0;                   /* GC index */
	emask = (ExposureMask | KeyPressMask | FocusChangeMask |
         	VisibilityChangeMask);
	XSelectInput(dpy, win, emask);
	while(multi != 0) { /* Async dispatch event with multic ground */
		event.type = 0;
		XCheckWindowEvent(dpy, win, emask,  &event);
		switch(event.type) {
			case Expose:   redraw(&event, gc[0], &r);
                  		   break;
			case VisibilityNotify: 
						   freeze = overlap(&event);
                           break;
     		case FocusIn:  rep5355(dpy, AutoRepeatModeOff);
                    	   break;
     		case FocusOut: rep5355(dpy, AutoRepeatModeOn);
                    	   break;
     		case KeyPress: if((multi = rapid(&event, delay)) > 0)
                       	   delay = multi;
                           break;
     		default:       break;
		} /* switch */

		if((multi < 0) || (freeze < 0))     /* Freeze display spiral  */
			continue;

		delay_func(50);
		XClearWindow(dpy, win);
		twist(dpy, win, gc[g], &r);         
		XDrawString(dpy, win, gc[g], 8, h, "CTRL-R", 6);
	} /* while event */


	{ /* Exit block */
		rep5355(dpy, AutoRepeatModeOn);
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
		return(0);
	} /* exit block */

} /* main */
