#include "EZX11.h"
#include <math.h>
#include <time.h>

long random(void);
void srandom(int seed);


#define radius 400
#define margin 20

#define Nmax 300

int N=10;

double Rinit=0.02;

double X[Nmax],Y[Nmax],R[Nmax],dx,dy,dw,dr;

  EZXW_p W;

void pol2xy(double w, double r, double *x, double *y)
{
  (*y) = r * sin(w);
  (*x) = r * cos(w);
}

void init(void)
{
  int i;
  double r,w;

  for(i=0; i<N; i++)
  {
    w = (random()%1000000)/1000000.0*2*M_PI;
    r = (random()%1000000)/1000000.0*(1.0-5*Rinit);

    if ((r+Rinit)>=1.0)
    {
      fprintf(stderr,"ohoh\n");
      exit(1);
    }

    pol2xy(w,r,&X[i],&Y[i]);

    R[i]=Rinit;
  }

}

void drawi(int i)
{
  EZX_FillCircle(W, margin+radius*(X[i]+1.0), margin+radius*(Y[i]+1.0),
	         radius*R[i]);
}

void draw(void)
{
  int i;

  for(i=0; i<N; i++)
    drawi(i);

}
          
int SQUARE=0;

void dmin(double x, double y, int i, int *f, double *dr)
{
  int j,mi;
  double m,dx,dy,d;

  mi=-1;
  if (! SQUARE)
    m=2*(1.0-sqrt(x*x+y*y));
  else
  {
    if (x<0.0)
    {       
      if (y<0.0)
        if (x<y)
          m=1.0+x;
        else
          m=1.0+y;
      else
        if (x<-y)
          m=1.0+x;
        else
          m=1.0-y;
    }   
    else
    {       
      if (y<0.0)
        if (-x<y)
          m=1.0-x;
        else
          m=1.0+y;
      else
        if (-x<-y)
          m=1.0-x;
        else
          m=1.0-y;
    }   

    m=2*m;
  }

  for(j=0; j<N; j++)
    if (i!=j)
    {
      dx = (x-X[j]);
      dy = (y-Y[j]);
      d = sqrt(dx*dx+dy*dy);

      if (d<m)
      {
        m = d;
        mi = j;
      }
    }

  *f = mi;
  *dr = m;
}

#define tr 16
double Tx[tr]={0.00,0.005,0.01,0.01,0.01,0.01,0.01,0.005,0.00,
               -0.005,-0.01,-0.01,-0.01,-0.01,-0.01,-0.005};
double Ty[tr]={0.01,0.01,0.01,0.005,0.00,-0.005,-0.01,-0.01,-0.01,-0.01,
               -0.01,-0.005,0.00,0.005,0.01,0.01};

int update(double f)
{
  int i,j,mi,ti,tj,updated=0;
  double di,dt;

  for(i=0; i<N; i++)
  {
    dmin(X[i],Y[i],i,&mi,&di);

    if (di<0)
    {
      fprintf(stderr,"no minimum found ... ?\n");
      exit(1);
    }

    tj=-1;
    for(j=0; j<tr; j++)
    {
      dmin(X[i]+f*Tx[j],Y[i]+f*Ty[j],i,&ti,&dt);
        
      if (dt>di)
      {
        tj=j;
        di=dt;
      }

    }

    if (tj>=0)
    {
      if (tj<0) fprintf(stderr,". %.2f>%.2f\n",di,4.0*R[i]*R[i]);

      drawi(i);

      if (tj>=0)
      {
        X[i]+=f*Tx[tj];
        Y[i]+=f*Ty[tj];
      }

      updated=1;

      drawi(i);
    }
  }

  return updated;
}

int main(int argc, char*argv[])
{
  int x,y;
  int frozen=0;
  char title[100];
  double f=1.0,eps,red;
  int seed;

  if (getenv("SQUARE"))
    SQUARE=1;

  if (argc<3)
  {
    fprintf(stderr,"Format: %s n r [seed [eps [red]]]    (z.B.: %s 12 0.01)\n",
            argv[0],argv[0]);
    exit(1);
  }

  N = atoi(argv[1]);
  Rinit = atof(argv[2]);
  eps = (argc<4) ? 0.001 : atof(argv[3]);
  red = (argc<5) ? 0.5 : atof(argv[4]);

  sprintf(title,"Cells");

  W = EZX_MakeWindow(title,2*(radius+margin),2*(radius+margin),NULL);

  EZX_SetMode(GXxor);
  EZX_SetColor(C_GREY50);
  
  if (!SQUARE)
    EZX_DrawCircle(W,radius+margin,radius+margin,radius);
  else
    EZX_DrawRectangle(W,margin,margin,2*radius,2*radius);

  seed = (argc>3) ? atoi(argv[3]) : 1234567891;

  srandom(seed);

  init();

  draw();

  do
  {
    if (! frozen)
    {
      if (! update(f))
      {
        if (f<eps)
          frozen=1;
        else
        {
          f*=red;

          sprintf(title,"Cells: new phase with factor %.3f",f);

          EZX_ChangeWindowTitle(W,title);
        }
      }

      if (frozen)
      {
        sprintf(title,"Cells [frozen]");

        EZX_ChangeWindowTitle(W,title);
      }
    }

    EZX_Flush();

    if (frozen)
      EZX_block_and_wait(NULL);
  }
  while (EZX_TestGetCursor(W,&x,&y)!=0);

  {
    int i,j;
    double m=10.0,d;
  
    for(i=0; i<N; i++)
    {
      dmin(X[i],Y[i],i,&j,&d);
  
      if (d<m)
        m=d;
    }
  
    draw();
  
    for(i=0; i<N; i++)
      R[i]=m/2.0;
  
    draw();
  
    sprintf(title,
    "%d cells [frozen]     seed: %d     maximal radius: %f",
    N,seed,m/2.0);
  
    EZX_ChangeWindowTitle(W,title);
  }

  EZX_GetAndForgetCursor();
  EZX_EndWindow(W);

  return 0;
}
