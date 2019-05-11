//
// Created by astronaut on 11.05.19.
//
#ifndef REFACTOR_PULSE_CIRCLE_H
#define REFACTOR_PULSE_CIRCLE_H

#include <X11/Xlib.h>
#include <time.h>

#define true 1
#define false 0

/* Semi-Circle (pi) spiral structure */

typedef struct {
	XPoint c[2];	 // 2 Semi-circles centeres c[1].x - c[0].x = R0
	int A;			 // Arc's coordinate (0 t - 180 - 360)
	int dA;			 // Arc in(de)crement (+1 or -1)
	int r_max;		 // Max spiral radius
	int radius_min;  // Min spiral radius
	int radius;		 // Semi-circle radius from 0 to r_max
	int dR;			 // radius in(de)crement (+R0 or -R0)
} XPiAr2;

/* Spiral Implementation function */

int max_size(XPiAr2 *pr, char *R0xN);  // check max spiral radius
int reset(XPiAr2 *);				 // Reset spiral from center & window height
int decent(XPiAr2 *);				 // xy-spiral centeres & window width
int redraw(XEvent *, GC, XPiAr2 *);  // redraw spiral fragment
int recent(XPiAr2 *pr, int is_inc);  // Change center for semi-circle
int twist(Display *dpy, Window win, GC gc, XPiAr2 *pr,
		  int *is_inc);		  // Rotate by spiral track
int rep5355(Display *, int);  // On/Off autorepeat + or - key
void get_event_code(XEvent *event, int *prev_code);	  // Control to spiral temp & freeze or exit
int overlap(XEvent *);		  // check visibility status

void delay_func(int number_of_seconds);

#endif  // REFACTOR_PULSE_CIRCLE_H
