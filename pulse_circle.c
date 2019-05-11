//
// Created by astronaut on 11.05.19.
//
#include "pulse_circle.h"
#include <X11/Xutil.h>
#include <stdio.h>
const int PAUSE = -1;
const int STOP = 0;
const int RESUME = 1;

/* Check maximal (extern) semi-circle radius */
int max_size(XPiAr2* pr, char* R0xN, unsigned int* width,
			 unsigned int* height) {
	int empty; /* for x & y */
	XParseGeometry(R0xN, &empty, &empty, (unsigned int*)width,
				   (unsigned int*)height);
	if (*height > *width) {
		pr->r_max = *width / 2;
	} else {
		pr->r_max = *height / 2;
	}
	pr->dR = pr->r_max * 0.05;
	pr->c.x = *width / 2;
	pr->c.y = *height / 2;
	pr->radius_min = 0;
	pr->radius = 0;
	return pr->r_max; /* return maximal radius */
}

/* Reset circle from center */
int reset(XPiAr2* pr) {
	pr->radius = 0;
	pr->radius_min = 0;
	if (pr->dR < 0) { pr->dR = -pr->dR; }
	return (2 * (pr->c.y)); /* return frame height */
}

/* redraw exposed circle fragment from center */
int redraw(XEvent* ev, GC gc, XPiAr2* pr) {
	int y;						/* ALT-X script y-location */
	XPiAr2 r;					/* circle copy */
	static XRectangle clip[32]; /* clip rectangles buffer */
	static int n = 0;			/* clip rectangles number */
	clip[n].x = (short)ev->xexpose.x;
	clip[n].y = (short)ev->xexpose.y;
	clip[n].width = (unsigned short)ev->xexpose.width;
	clip[n].height = (unsigned short)ev->xexpose.height;
	n++;
	if ((ev->xexpose.count > 0) && (n < 32)) return (0);
	XSetClipRectangles(ev->xexpose.display, gc, 0, 0, clip, n, Unsorted);
	r = *pr; /* = memcpy(&r, pr, (sizeof(XPiAr2))); */
	y = reset(&r);
	//  while (twist(ev->xexpose.display, ev->xexpose.window, gc, &r) <
	//  pr->radius)
	//    ;
	int is_inc = 1;
	twist(ev->xexpose.display, ev->xexpose.window, gc, &r, &is_inc);
	XDrawString(ev->xexpose.display, ev->xexpose.window, gc, 8, y, "CTRL-C", 5);
	XSetClipMask(ev->xexpose.display, gc, None);
	return (n = 0);
}

/* Change center for semi-circle (semi-step) */
int recent(XPiAr2* pr, int is_inc) {
	if (is_inc) {
		pr->radius += pr->dR;
	} else {
		pr->radius -= pr->dR;
	}
	return 0;
}

/* 1 arc step by circle track */
int twist(Display* dpy, Window win, GC gc, XPiAr2* pr, int* is_inc) {
	int i = recent(pr, *is_inc);
	XDrawArc(dpy, win, gc, pr->c.x - pr->radius, pr->c.y - pr->radius,
			 (unsigned int)(2 * pr->radius), (unsigned int)(2 * pr->radius), 0,
			 64 * 360);
	XFlush(dpy);
	//  amod2pi(pr);
	return (pr->radius);
}

/* Turn On/Off autorepeat plus or minus key */
int rep5355(Display* dpy, int r) {
	XKeyboardControl kbval;
	kbval.key = XKeysymToKeycode(dpy, XK_KP_Add);
	kbval.auto_repeat_mode = r;
	XChangeKeyboardControl(dpy, (KBKey | KBAutoRepeatMode), &kbval);
	kbval.key = XKeysymToKeycode(dpy, XK_KP_Subtract);
	XChangeKeyboardControl(dpy, (KBKey | KBAutoRepeatMode), &kbval);
	return (r);
}

/* Keyboard Control to ac(de)celerate circle get_event_code or exit */
void get_event_code(XEvent* event, int* prev_code) {
	KeySym code[1]; /* key pressed symbol X code */
	XLookupString((XKeyEvent*)event, NULL, 0, code, NULL);
	switch (code[0]) {
		case XK_Pause: {
			*prev_code = PAUSE;
			break;
		}
		case XK_Return: {
			*prev_code = RESUME;
			break;
		}
		case XK_C:
		case XK_c: {
			if (event->xkey.state & ControlMask) {
				*prev_code = STOP; /* Ctrl-C to exit */
				break;
			}
		}
	}
}

/* Check Main Window Visibility to (un)freeze */
int overlap(XEvent* ev) {
	if (ev->xvisibility.state != VisibilityUnobscured) return (-32);
	return (0);
}

void delay_func(int number_of_seconds) {
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds) {};
}

