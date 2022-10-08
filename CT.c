
#include "EZX11.h"

main()
  {
  int i,x,y;
  EZXW_p w;

if (0)
  {
  EZXW_p H;

  H = EZX_MakeWindow("Horvert",10,10,"+100+100");
  EZX_GetAndForgetCursor();

  EZX_EndWindow(H);
  }

  w = EZX_MakeWindow("ColorSelect",900,600,"+50+50");

  for(i=0; i<54; i++)
    {
    EZX_SetColor(i);

    EZX_FillRectangle(w,100*(i%9),100*(i/9),100,100);
    }

  EZX_GetCursor(&x,&y);

  EZX_EndWindow(w);

  printf("You selected color with number %d.\n",(y/100)*9+(x/100));

  return 0;

  }

