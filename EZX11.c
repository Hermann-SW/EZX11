#include <assert.h>

int return_old_assign(int *old, int new) {
  int save = *old;
  return *old = new, save;
}

#include "ezxdisp.h"

#include <X11/Xutil.h>
struct ezx_s {
  int size_x, size_y;
  ezx_color_t bgcolor;
  int closed;

  Display     *display;
  Window       top;
  XFontStruct *fontst;
  GC           gc;
  Colormap     cmap;
  XColor       black, white, rgb;
  Pixmap       pixmap;
  XSizeHints   size_hints;
  Atom         wm_protocols, wm_delete_window;

  int                 cur_layer;
  struct ezx_figures *fig_head[EZX_NLAYER];
  struct ezx_figures *fig_tail[EZX_NLAYER];

  // 3d stuffs
  double scrdist, mag;
  double eye_x, eye_y, eye_z, eye_r;
  double cmat[3][3];
  double light_x, light_y, light_z;

  struct color_table_s *color_table;
};

#include "EZX11.h"

int EZX_col=0;

#define EZX_f(c) {(c>>16)/255.0,((c>>8)&0xFF)/255.0,(c&0xFF)/255.0}

ezx_color_t EZX_cols[54] = {EZX_f(0xffffff),EZX_f(0x0),EZX_f(0xff0000),
EZX_f(0x4eee94),EZX_f(0xff),EZX_f(0xffff00),EZX_f(0xffd700),EZX_f(0xee82ee),
EZX_f(0xffc0cb),EZX_f(0x0),EZX_f(0xd0d0d),EZX_f(0x1a1a1a),EZX_f(0x262626),
EZX_f(0x333333),EZX_f(0x404040),EZX_f(0x4d4d4d),EZX_f(0x595959),
EZX_f(0x666666),EZX_f(0x737373),EZX_f(0x7f7f7f),EZX_f(0x8c8c8c),
EZX_f(0x999999),EZX_f(0xa6a6a6),EZX_f(0xb3b3b3),EZX_f(0xbfbfbf),
EZX_f(0xcccccc),EZX_f(0xd9d9d9),EZX_f(0xe5e5e5),EZX_f(0xf2f2f2),
EZX_f(0xffffff),EZX_f(0xc71585),EZX_f(0x8968cd),EZX_f(0x548b54),
EZX_f(0xffff),EZX_f(0x36648b),EZX_f(0x8b2500),EZX_f(0x8b864e),EZX_f(0xced1),
EZX_f(0x228b22),EZX_f(0xfdf5e6),EZX_f(0x778899),EZX_f(0x708090),
EZX_f(0x2f4f4f),EZX_f(0x668b8b),EZX_f(0x32cd32),EZX_f(0x5f9ea0),
EZX_f(0xff1493),EZX_f(0xee00ee),EZX_f(0x8b4726),EZX_f(0xffb5c5),
EZX_f(0x868b),EZX_f(0x4169e1),EZX_f(0xeeee00),EZX_f(0xbdc1ff)};


ezx_t *EZX_MakeWindow(char *title, int width, int height, char *position)
{
  return ezx_init(width, height, title);
}


void EZX_FillRectangle(ezx_t *e, int x, int y, unsigned w, unsigned h) {
   ezx_fillrect_2d(e, x, y, x+w, y+h, EZX_cols+EZX_col);
}


int  EZX_GetCursor    	(EZXW_p w, int *xp, int *yp) {
  ezx_event_t ev;
  ezx_redraw(w);
  do {
    ezx_next_event(w, &ev);
  }
  while (ev.type != EZX_BUTTON_PRESS);
  *xp = ev.button.x;
  *yp = ev.button.y;

    if (ev.button.b == Button1) return(LEFT_BUTTON);
  else if (ev.button.b == Button2) return(MIDDLE_BUTTON);
  else if (ev.button.b == Button3) return(RIGHT_BUTTON);

  return(OTHER_BUTTON);
}


void EZX_EndWindow(EZXW_p w) {
  XFlush(w->display);
  XDestroyWindow(w->display, w->top);
  XFlush(w->display);
}


int  EZX_SetColor(int color) {
  assert((0 <= color) && (color < MAXCOLORS));
  return return_old_assign(&EZX_col, color);
}

