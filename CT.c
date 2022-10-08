
#include "EZX11.h"

int main()
  {
  int i,x,y;
  EZXW_p w;

  w = EZX_MakeWindow("ColorSelect",900,600,"+50+50");

  for(i=0; i<54; i++)
    {
    EZX_SetColor(i);

    EZX_FillRectangle(w,100*(i%9),100*(i/9),100,100);
    }

  EZX_GetCursor(w,&x,&y);

  EZX_EndWindow(w);

  printf("You selected color with number %d.\n",(y/100)*9+(x/100));

  return 0;

  }

