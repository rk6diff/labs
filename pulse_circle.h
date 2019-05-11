//
// Created by astronaut on 11.05.19.
//
#ifndef REFACTOR_PULSE_CIRCLE_H
#define REFACTOR_PULSE_CIRCLE_H

#include <X11/Xlib.h>
#include <time.h>

#define true 1
#define false 0

/* Semi-Circle (pi) circle structure */

typedef struct {
	XPoint c;		 // 2 Semi-circles centeres c[1].x - c[0].x = R0
	int r_max;		 // Max circle radius
	int radius_min;  // Min circle radius
	int radius;		 // Semi-circle radius from 0 to r_max
	int dR;			 // radius in(de)crement (+R0 or -R0)
} XPiAr2;

/* Spiral Implementation function */

int max_size(XPiAr2 *pr, char *R0xN, unsigned int *width,
			 unsigned int *height);  // check max circle radius
int reset(XPiAr2 *);				 // Reset circle from center & window height
int redraw(XEvent *, GC, XPiAr2 *);  // redraw circle fragment
int recent(XPiAr2 *pr, int is_inc);  // Change center for semi-circle
int twist(Display *dpy, Window win, GC gc, XPiAr2 *pr,
		  int *is_inc);		  // Rotate by circle track
int rep5355(Display *, int);  // On/Off autorepeat + or - key
void get_event_code(XEvent *event,
					int *prev_code);  // Control to circle temp & freeze or exit
int overlap(XEvent *);				  // check visibility status

void delay_func(int number_of_seconds);

#endif  // REFACTOR_PULSE_CIRCLE_H

