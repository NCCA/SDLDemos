/* a basic window in SDL use the command line
g++ -Wall -g TriangleRender.cpp -o TriangleRender `sdl-config --cflags --libs`

or

clang++ -Wall -g TriangleRender.cpp -o TriangleRender `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>

/// @brief the width of the window
const int WINDOW_WIDTH = 1024;
/// @brief the height of the window
const int WINDOW_HEIGHT = 720;

/// @brief simple SDL clear screen function
/// @param [in] _screen a pointer to the SDL screen structure
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void clearScreen(
									SDL_Surface *_screen,
									char _r,
									char _g,
									char _b
								);

/// @brief simple SDL clear screen function
/// @param [in] _screen a pointer to the SDL screen structure
/// @param [in] _x position of the rectangle
/// @param [in] _y position of the rectangle
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void putPixel(
							SDL_Surface *_screen,
							int _x,
							int _y,
							char _r,
							char _g,
							char _b
						);

void gouraudTriangle(SDL_Surface *_screen,
                        int x0, int y0, char r0, char g0, char b0,
                              int x1, int y1, char r1, char g1, char b1,
                              int x2, int y2, char r2, char g2, char b2);

/// @brief return a random value in the range _min -> _max
/// @param [in] _min the min value
/// @param [in] _max the max value
/// @returns a random value from _min -> _max
int randomRange(
								int _min,
								int _max
								);

/// note that under mac osx (and windows) there is a different
/// way to build SDL so we need to use SDL_main under linux
/// normal main is fine so we use this conditional compilation
/// to incude the correct version (DARWIN being mac os x)
#if defined (DARWIN) || defined (WIN32)
  int SDL_main(int argc, char **argv)
#else
  int main(int argc, char **argv)
#endif
{
// First thing we need to do is initialise SDL in this case we are
	// setting up just the video subsystem if we need audio or timer etc
	// we would | (or) the flags together see http://www.libsdl.org/intro.en/usinginit.html
	// we check the return value and if not 0 it is an error
	if (SDL_Init( SDL_INIT_VIDEO ) !=0)
	{
		std::cerr <<"error initialising SDL exiting\n";
		exit(EXIT_FAILURE);
	}
	// now we are going to create the video mode for our window. In this
	// case we are going to create a screen width / heigh
	// the 3rd parameter is Bits per Pixel if we set to 0 it will use the
	// current screen default value
	// Finally we pass the flags to say what type of screen we need. In this case
	// we are setting a Hardware surface using double buffering, in later
	// examples we will use OpenGL setting so we can access GL stuff
	// for more info see here http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode
	SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
	if( screen == NULL)
	{
		std::cerr<<"error setting SDL Video Mode\n";
		exit(EXIT_FAILURE);
	}

	// next we set the window bar caption to the text 2nd param is for an icon
	// this is a char * to a pixmap data but if we use 0 none is loaded
	SDL_WM_SetCaption( "Bresenham's line algorithm", 0 );
	// clear screen to white
	clearScreen(screen,255,255,255);

	SDL_Event event;

	bool quit=false;
	// now we loop until the quit flag is set to true
	while(!quit)
	{
		// generate random colour values from 0-255
		char r1=randomRange(0,255);
		char g1=randomRange(0,255);
		char b1=randomRange(0,255);
		char r2=randomRange(0,255);
		char g2=randomRange(0,255);
		char b2=randomRange(0,255);
		char r3=randomRange(0,255);
		char g3=randomRange(0,255);
		char b3=randomRange(0,255);
		int x0=randomRange(0,WINDOW_WIDTH);
		int y0=randomRange(0,WINDOW_HEIGHT);
		int x1=randomRange(0,WINDOW_WIDTH);
		int y1=randomRange(0,WINDOW_HEIGHT);
		int x2=randomRange(0,WINDOW_WIDTH);
		int y2=randomRange(0,WINDOW_HEIGHT);
 		gouraudTriangle(screen,x0,y0,r1,g1,b1,x1,y1,r2,g2,b2,x2,y2,r2,g2,b2);
		SDL_Flip(screen);
		sleep(1);

		// process SDL events, in this case we are looking for keys
		while ( SDL_PollEvent(&event) )
		{
			switch (event.type)
			{
				// this is the window x being clicked.
				case SDL_QUIT : quit = true; break;

				// now we look for a keydown event
				case SDL_KEYDOWN:
				{
					switch( event.key.keysym.sym )
					{
						// if it's the escape key quit
						case SDLK_ESCAPE :  quit = true; break;
						default : break;
					}
				}

				default : break;
			}
		}

	} // end processing loop

	// finally when we are done we need to tidy up SDL by calling SDL_Quit
	// sometime this is added as the atexit function to make it happen
	// automatically
	SDL_Quit();

	return EXIT_SUCCESS;
}


int randomRange(
								int _min,
								int _max
								)
{
	return (int)((double)rand() / ((double)RAND_MAX + _min) * _max);
}


void clearScreen(
									SDL_Surface *_screen,
									char _r,
									char _g,
									char _b
								)
{
	// first we need to convert the rgb triple into the correct int value
	// this is based on the current video Bits per pixel which we grab
	// from the video surface passed in
	Uint32 colour =SDL_MapRGB(_screen->format,_r,_g,_b);
	// next we fill the screen using the SDL_FillRect function,
	// the 2nd parameter is SDL_Rect *dstrect this is a rectangle to fill
	// if it's set to NULL it indicates the whole screen
	SDL_FillRect( _screen, 0, colour );
	// finally we need to tell SDL to Flip and update the screen
	SDL_Flip(_screen);
}


void putPixel(
								SDL_Surface *_screen,
								int _x,
								int _y,
								char _r,
								char _g,
								char _b
							)
{
	SDL_Rect rect;
	rect.x=_x;
	rect.y=_y;
	rect.w=1;
	rect.h=1;
	// first we need to convert the rgb triple into the correct int value
	// this is based on the current video Bits per pixel which we grab
	// from the video surface passed in
	Uint32 colour =SDL_MapRGB(_screen->format,_r,_g,_b);
	// next we fill the screen using the SDL_FillRect function,
	// the 2nd parameter is SDL_Rect *dstrect this is a rectangle to fill
	// if it's set to NULL it indicates the whole screen
	SDL_FillRect( _screen, &rect, colour );

}

// modified from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void drawLine(
							SDL_Surface *_screen,
							int _x0,
							int _y0,
							int _x1,
							int _y1,
							char _r,
							char _g,
							char _b
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
			putPixel(_screen,y,x,_r,_g,_b);
		}
		else
		{
			putPixel(_screen,x,y,_r,_g,_b);
		}
		error-=deltay;
		if(error <0)
		{
			y+=ystep;
			error+=deltax;
		}
	}
	SDL_Flip(_screen);
}

void gouraudTriangle(SDL_Surface *_screen,
                        int x0, int y0, char r0, char g0, char b0,
                              int x1, int y1, char r1, char g1, char b1,
                              int x2, int y2, char r2, char g2, char b2)
{
      // Sort our points into order of y
      // 0 top
      // 2 middle
      // 1 bottom
      if( y1 < y0 )
      {
            std::swap(y1, y0);
            std::swap(x1, x0);
            std::swap(r1, r0);  std::swap(g1, g0);  std::swap(b1, b0);
      }
      if( y2 < y0 )
      {
            std::swap(y2, y0);
            std::swap(x2, x0);
            std::swap(r2, r0);  std::swap(g2, g0);  std::swap(b2, b0);
      }
      if( y1 < y2 )
      {
            std::swap(y2, y1);
            std::swap(x2, x1);
            std::swap(r2, r1);  std::swap(g2, g1);  std::swap(b2, b1);
      }


      float xl_edge = (float)x0;  // left edge
      float xr_edge = (float)x0;  // right edge


      float dxldy;
      float dxrdy;


      float dxdy1 = (float)(x2-x0)/(y2-y0);
      float dxdy2 = (float)(x1-x0)/(y1-y0);

      float dr1 = (float)(r2-r0)/(y2-y0);
      float dg1 = (float)(g2-g0)/(y2-y0);
      float db1 = (float)(b2-b0)/(y2-y0);

      float dr2 = (float)(r1-r0)/(y1-y0);
      float dg2 = (float)(g1-g0)/(y1-y0);
      float db2 = (float)(b1-b0)/(y1-y0);

      float drldy, dgldy, dbldy;
      float drrdy, dgrdy, dbrdy;

      if( dxdy1 < dxdy2 )
      {
            dxldy = dxdy1;
            dxrdy = dxdy2;

            drldy  = dr1;     dgldy  = dg1;     dbldy  = db1; // left  (r,g,b)
            drrdy  = dr2;     dgrdy  = dg2;     dbrdy  = db2; // right (r,g,b)

      }
      else
      {
            dxldy = dxdy2;
            dxrdy = dxdy1;

            drldy  = dr2;     dgldy  = dg2;     dbldy  = db2; // left  (r,g,b)
            drrdy  = dr1;     dgrdy  = dg1;     dbrdy  = db1; // right (r,g,b)
      }

      float r_left  = r0;
      float r_right = r0;
      float g_left  = g0;
      float g_right = g0;
      float b_left  = b0;
      float b_right = b0;

      // Top of the triangle
      for(int y=y0; y<y2; y++)
      {

            float dr = (r_right - r_left)/(xr_edge - xl_edge);
            float dg = (g_right - g_left)/(xr_edge - xl_edge);
            float db = (b_right - b_left)/(xr_edge - xl_edge);

            float pr = r_left;
            float pg = g_left;
            float pb = b_left;

            for(int x=xl_edge; x<xr_edge; x++)
            {
                  pr = pr + dr;
                  pg = pg + dg;
                  pb = pb + db;

                  putPixel(_screen,
                             x, y, pr*255,pg*255,pb*255 );
            }//end for loop x

            xl_edge = xl_edge + dxldy;
            xr_edge = xr_edge + dxrdy;

            r_left  += drldy;
            r_right += drrdy;
            g_left  += dgldy;
            g_right += dgrdy;
            b_left  += dbldy;
            b_right += dbrdy;

      }// end for loop y


      // Bottom half of the triangle

      if( dxdy1 < dxdy2 )
      {
            dxldy = (float)(x2-x1)/(y2-y1);

            drldy  = (r2-r1)/(y2-y1);
            dgldy  = (g2-g1)/(y2-y1);
            dbldy  = (b2-b1)/(y2-y1);
      }
      else
      {
            dxrdy = (float)(x2-x1)/(y2-y1);

            drrdy  = (r2-r1)/(y2-y1);
            dgrdy  = (g2-g1)/(y2-y1);
            dbrdy  = (b2-b1)/(y2-y1);
      }

      for(int y=y2; y<y1; y++)
      {

            float dr = (r_right - r_left)/(xr_edge - xl_edge);
            float dg = (g_right - g_left)/(xr_edge - xl_edge);
            float db = (b_right - b_left)/(xr_edge - xl_edge);

            float pr = r_left;
            float pg = g_left;
            float pb = b_left;

            for(int x=xl_edge; x<xr_edge; x++)
            {
                  pr = pr + dr;
                  pg = pg + dg;
                  pb = pb + db;

                  putPixel(_screen,x, y, pr*255,pg*255,pb*255);
            }//end for loop x

            xl_edge = xl_edge + dxldy;
            xr_edge = xr_edge + dxrdy;

            r_left  += drldy;
            r_right += drrdy;
            g_left  += dgldy;
            g_right += dgrdy;
            b_left  += dbldy;
            b_right += dbrdy;

      }// end for loop y

}// End of Draw_Gouraud_Triangle(..)


