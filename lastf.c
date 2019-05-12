#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <stdio.h>
#include <string.h>
#include "lasth.h"

/* Check maximal (extern) circle radius */

int maxisize(XPiAr2* pr, char* R0xN, unsigned int* w, unsigned int* h) {
	int empty;                      /* for x & y */ 
	XParseGeometry(R0xN, &empty, &empty, w, h);
	if((*w==*h)||(*w<*h)) pr->Rm=*w/2;
		else pr->Rm=*h/2;
	pr->dR=pr->Rm*0.01;
	pr->c.x=*w/2;
	pr->c.y=*h/2;
	pr->R=0;
	return(pr->Rm);
} /* maxisize */

/* Reset circle from center */

int reset(XPiAr2* pr) {
	pr->R = 0; 
	if(pr->dR < 0)
		pr->dR = -pr->dR;
	return(2*(pr->c.y)); /* return frame height */
} /* reset */

/* redraw exposed circle fragment from center */

int redraw(XEvent* ev, GC gc, XPiAr2* pr) {
	int y;                          /* ALT-X script y-location */
	XPiAr2 r;                       /* spiral copy */
	static XRectangle clip[32];     /* clip rectangles buffer */
	static int n = 0;               /* clip rectangles number */
	clip[n].x = ev->xexpose.x;
	clip[n].y = ev->xexpose.y;
	clip[n].width =  ev->xexpose.width;
	clip[n].height =  ev->xexpose.height;
	n++;
	if((ev->xexpose.count > 0) && (n < 32))
		return(0);
	XSetClipRectangles(ev->xexpose.display, gc, 0, 0, clip, n, Unsorted);
	r = *pr; /* = memcpy(&r, pr, (sizeof(XPiAr2))); */ 
	y = reset(&r) - 8;
	while(twist(ev->xexpose.display, ev->xexpose.window, gc, &r) < pr->R);
	twist(ev->xexpose.display, ev->xexpose.window, gc, &r);
	XDrawString(ev->xexpose.display, ev->xexpose.window, gc, 
            8, y, "CTRL-R", 6);
	XSetClipMask(ev->xexpose.display, gc, None);
	return(n=0);
} /* redraw */

int recent(XPiAr2* pr) {
	if(pr->R<pr->Rm) 
		pr->R+=pr->dR; 
	else {
        pr->R=1;
        printf("\a"); //SOUND. NEED TO REPLACE. (doesn`t work((( )
         }
	return(0);
}

/* 1 circle step */

int twist(Display* dpy, Window win, GC gc, XPiAr2* pr) {
	recent(pr);
	XFillArc(dpy, win, gc, pr->c.x - pr->R, pr->c.y - pr->R,
         (2*pr->R), (2*pr->R), 0, 64*360);
	XFlush(dpy);
	return(pr->R);
} /* twist */

/* Turn On/Off autorepeat plus or minus key */

int rep5355(Display* dpy, int r) {
	XKeyboardControl kbval;
	kbval.key = XKeysymToKeycode(dpy, XK_KP_Add);
	kbval.auto_repeat_mode = r;
	XChangeKeyboardControl(dpy, (KBKey | KBAutoRepeatMode), &kbval);
	kbval.key = XKeysymToKeycode(dpy, XK_KP_Subtract);
	XChangeKeyboardControl(dpy, (KBKey | KBAutoRepeatMode), &kbval);
	return(r);
} /* rep5355 */

/* Keyboard Control to ac(de)celerate spiral rapid or exit */

int rapid(XEvent* ev, int t) {
	KeySym code[1];            /* key pressed symbol X code */
	XLookupString((XKeyEvent* ) ev, NULL, 0, code, NULL);
	switch(code[0]) {
		case XK_plus:   /* accelerate tempo or decrease delay */
		case XK_KP_Add: if(t==-32) t=1;
		  				if(t < 10) 
                   		++t;
                  		break;
		case XK_minus: /* decerarate tempo or  increase delay */                
		case XK_KP_Subtract: 
						if(t==0) t=-32;
						else
			   				t--;
                        break;
		case XK_r:
		case XK_R:      if(ev->xkey.state & ControlMask)
                   		t = 0;  /* Ctrl-R to exit */
                 		break; 
		default:        break; /* unfreeze spiral */
	} /* switch */
	return(t);
} /* rapid */

/* Check Main Window Visibility to (un)freeze */

int overlap(XEvent* ev) {
	if(ev->xvisibility.state != VisibilityUnobscured)
		return(-32);
	return(0);
} /* overlap */

void delay_func(int number_of_seconds) {
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds) {};
}
