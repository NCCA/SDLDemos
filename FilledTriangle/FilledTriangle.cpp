/* a basic window in SDL use the command line
g++ -Wall -g FilledTriangle.cpp -o FilledTriangle `sdl-config --cflags --libs`

or

clang++ -Wall -g FilledTriangle.cpp -o FilledTriangle `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL.h>
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

/// @brief draw a filled triangle with one colour modified from
/// http://www.xbdev.net/maths_of_3d/rasterization/gouraudtriangle/index.php
/// @brief simple SDL clear screen function
/// @param [in] _screen a pointer to the SDL screen structure
/// @param [in] _x0 position of the first vertex
/// @param [in] _y0 position of the first vertex
/// @param [in] _x1 position of the second vertex
/// @param [in] _y1 position of the second vertex
/// @param [in] _x2 position of the third vertex
/// @param [in] _y2 position of the third vertex
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255

void filledTriangle(
										SDL_Surface *_screen,
										int _x0,
										int _y0,
										int _x1,
										int _y1,
										int _x2,
										int _y2,
										char _r,
										char _g,
										char _b
										);

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
	SDL_WM_SetCaption( "Triangle Rendering", 0 );
	// clear screen to white
	clearScreen(screen,255,255,255);

	SDL_Event event;

	bool quit=false;
	// now we loop until the quit flag is set to true
	while(!quit)
	{
		// generate random colour values from 0-255
		char r=randomRange(0,255);
		char g=randomRange(0,255);
		char b=randomRange(0,255);
		int x0=randomRange(0,WINDOW_WIDTH);
		int y0=randomRange(0,WINDOW_HEIGHT);
		int x1=randomRange(0,WINDOW_WIDTH);
		int y1=randomRange(0,WINDOW_HEIGHT);
		int x2=randomRange(0,WINDOW_WIDTH);
		int y2=randomRange(0,WINDOW_HEIGHT);
 		filledTriangle(screen,x0,y0,x1,y1,x2,y2,r,g,b);
		SDL_Flip(screen);

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


void filledTriangle(
										SDL_Surface *_screen,
										int _x0,
										int _y0,
										int _x1,
										int _y1,
										int _x2,
										int _y2,
										char _r,
										char _g,
										char _b
										)
{
      // Sort our points into order of y
      // 0 top
      // 2 middle
      // 1 bottom
      if( _y1 < _y0 )
      {
            std::swap(_y1, _y0);
            std::swap(_x1, _x0);
      }
      if( _y2 < _y0 )
      {
            std::swap(_y2, _y0);
            std::swap(_x2, _x0);
      }
      if( _y1 < _y2 )
      {
            std::swap(_y2, _y1);
            std::swap(_x2, _x1);
      }


      float xl_edge = (float)_x0;  // left edge
      float xr_edge = (float)_x0;  // right edge


      float dxldy;
      float dxrdy;


      float dxd_y1 = (float)(_x2-_x0)/(_y2-_y0);
      float dxd_y2 = (float)(_x1-_x0)/(_y1-_y0);




      if( dxd_y1 < dxd_y2 )
      {
            dxldy = dxd_y1;
            dxrdy = dxd_y2;


      }
      else
      {
            dxldy = dxd_y2;
            dxrdy = dxd_y1;

      }


      // Top of the triangle
      for(int y=_y0; y<_y2; y++)
      {

            for(int x=xl_edge; x<xr_edge; x++)
            {

                  putPixel(_screen,
                             x, y, _r,_g,_b );
            }//end for loop x

            xl_edge = xl_edge + dxldy;
            xr_edge = xr_edge + dxrdy;

      }// end for loop y


      // Bottom half of the triangle

      if( dxd_y1 < dxd_y2 )
      {
            dxldy = (float)(_x2-_x1)/(_y2-_y1);
      }
      else
      {
            dxrdy = (float)(_x2-_x1)/(_y2-_y1);

      }

      for(int y=_y2; y<_y1; y++)
      {

            for(int x=xl_edge; x<xr_edge; x++)
            {

                  putPixel(_screen,x, y, _r,_g,_b);
            }//end for loop x

            xl_edge = xl_edge + dxldy;
            xr_edge = xr_edge + dxrdy;

      }// end for loop y

}// End of Draw_Gouraud_Triangle(..)


