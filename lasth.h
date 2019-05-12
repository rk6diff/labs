#ifndef LAB4_LASTH_H_ // эт не просто для красоты
#define LAB4_LASTH_H_

#include <time.h>

#define true 1
#define false 0
#define MIN_SPEED 10

typedef struct {
  XPoint c;  /* 2 Semi-circles centeres c[1].x - c[0].x = R0 */
  int Rm;       /* Max spiral radius */
  int R;        /* Semi-circle radius from 0 to Rm */
  int dR;       /* radius in(de)crement (+R0 or -R0) */ 
} XPiAr2;

/* Spiral Implementation function */

int maxisize(XPiAr2*, char*, unsigned int*, unsigned int*);  /* check max spiral radius */
int reset(XPiAr2*);          /* Reset spiral from center & window height */
int redraw(XEvent*, GC, XPiAr2*);              /* redraw spiral fragment */
int recent(XPiAr2*);                    /* Change center for semi-circle */
int twist(Display*, Window, GC, XPiAr2*);      /* Rotate by spiral track */
int rep5355(Display*, int);              /* On/Off autorepeat + or - key */
int rapid(XEvent*, int);      /* Control to spiral temp & freeze or exit */
int overlap(XEvent*);                         /* check visibility status */
void delay_func(int number_of_seconds);

#endif  // LAB4_LASTH_H_