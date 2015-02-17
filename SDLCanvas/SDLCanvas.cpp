#include "SDLCanvas.h"
#include <iostream>
SDLCanvas* SDLCanvas::m_pinstance = 0;// initialize pointer

SDLCanvas*  SDLCanvas::instance()
{
  if (m_pinstance == 0)  // is it the first call?
  {
    m_pinstance = new SDLCanvas; // create sole instance
  }
  return m_pinstance; // address of sole instance
}


void SDLCanvas::setScreen(int _w, int _h)
{
  m_width=_w;
  m_height=_h;
  // now we are going to create the video mode for our window. In this
  // case we are going to create a screen width / heigh
  // the 3rd parameter is Bits per Pixel if we set to 0 it will use the
  // current screen default value
  // Finally we pass the flags to say what type of screen we need. In this case
  // we are setting a Hardware surface using double buffering, in later
  // examples we will use OpenGL setting so we can access GL stuff
  // for more info see here http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode
  m_screen = SDL_SetVideoMode( _w, _h, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
  if( m_screen == NULL)
  {
    std::cerr<<"error setting SDL Video Mode\n";
    exit(EXIT_FAILURE);
  }
}



SDLCanvas::SDLCanvas()
{
  m_width=1024;
  m_height=720;
  m_r=0;
  m_g=0;
  m_b=0;
  m_bgR=255;
  m_bgG=255;
  m_bgB=255;
  m_pointSize=1;
  // First thing we need to do is initialise SDL in this case we are
  // setting up just the video subsystem if we need audio or timer etc
  // we would | (or) the flags together see http://www.libsdl.org/intro.en/usinginit.html
  // we check the return value and if not 0 it is an error
  if (SDL_Init( SDL_INIT_VIDEO ) !=0)
  {
    std::cerr <<"error initialising SDL exiting\n";
    exit(EXIT_FAILURE);
  }

  if ( TTF_Init() < 0 )
  {
    std::cerr<<"Couldn't initialize TTF: "<<SDL_GetError()<<"\n";
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  m_font = TTF_OpenFont("Arial.ttf", 16);


}

void SDLCanvas::renderText(int _x, int _y, const std::string &_c)
{
SDL_Color *forecol;
forecol->r=m_r;
forecol->g=m_g;
forecol->b=m_b;
SDL_Rect dstrect;
SDL_Surface *text = TTF_RenderText_Solid(m_font, _c.c_str(), *forecol);
dstrect.x = _x;
dstrect.y = _y;
dstrect.w = text->w;
dstrect.h = text->h;
SDL_BlitSurface(text, NULL, m_screen, &dstrect);
SDL_FreeSurface(text);
}

void SDLCanvas::clearScreen()
{
  // first we need to convert the rgb triple into the correct int value
  // this is based on the current video Bits per pixel which we grab
  // from the video surface passed in
  Uint32 colour =SDL_MapRGB(m_screen->format,m_bgR,m_bgG,m_bgB);
  // next we fill the screen using the SDL_FillRect function,
  // the 2nd parameter is SDL_Rect *dstrect this is a rectangle to fill
  // if it's set to NULL it indicates the whole screen
  SDL_FillRect( m_screen, 0, colour );
  // finally we need to tell SDL to Flip and update the screen
}


void SDLCanvas::putPixel(
                          int _x,
                          int _y
                        )
{
  SDL_Rect rect;
  rect.x=_x;
  rect.y=_y;
  rect.w=m_pointSize;
  rect.h=m_pointSize;
  // first we need to convert the rgb triple into the correct int value
  // this is based on the current video Bits per pixel which we grab
  // from the video surface passed in
  Uint32 colour =SDL_MapRGB(m_screen->format,m_r,m_g,m_b);
  // next we fill the screen using the SDL_FillRect function,
  // the 2nd parameter is SDL_Rect *dstrect this is a rectangle to fill
  // if it's set to NULL it indicates the whole screen
  SDL_FillRect( m_screen, &rect, colour );

}
// from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void SDLCanvas::drawCircle(
                            int _x0,
                            int _y0,
                            int _radius
                           )
{
  int f = 1 - _radius;
  int ddF_x = 1;
  int ddF_y = -2 * _radius;
  int x = 0;
  int y = _radius;

  putPixel(_x0, _y0 + _radius);
  putPixel(_x0, _y0 - _radius);
  putPixel(_x0 + _radius, _y0);
  putPixel(_x0 - _radius, _y0);

  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    putPixel(_x0 + x, _y0 + y);
    putPixel(_x0 - x, _y0 + y);
    putPixel(_x0 + x, _y0 - y);
    putPixel(_x0 - x, _y0 - y);
    putPixel(_x0 + y, _y0 + x);
    putPixel(_x0 - y, _y0 + x);
    putPixel(_x0 + y, _y0 - x);
    putPixel(_x0 - y, _y0 - x);
  }
}





// modified from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void SDLCanvas::drawLine(
                        int _x0,
                        int _y0,
                        int _x1,
                        int _y1
                       )
{
  bool steep = abs(_y1 - _y0) > abs(_x1 - _x0);
  if(steep)
  {
    std::swap(_x0,_y0);
    std::swap(_x1,_y1);
  }
  if( _x0> _x1)
  {
    std::swap(_x0,_x1);
    std::swap(_y0,_y1);
  }
  int deltax = _x1-_x0;
  int deltay = abs(_y1-_y0);
  int error = deltax/2;
  int ystep;
  int y = _y0;
  if(_y0 < _y1)
  {
    ystep=1;
  }
  else
  {
    ystep=-1;
  }
  for(int x=_x0; x<_x1; ++x)
  {
    if(steep)
    {
      putPixel(y,x);
    }
    else
    {
      putPixel(x,y);
    }
    error-=deltay;
    if(error <0)
    {
      y+=ystep;
      error+=deltax;
    }
  }
}


