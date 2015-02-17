/* a basic window in SDL use the command line
g++ -Wall -g RandomRectangle.cpp -o RandomRectangle `sdl-config --cflags --libs`

or

clang++ -Wall -g RandomRectangle.cpp -o RandomRectangle `sdl-config --cflags --libs`

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
/// @param [in] _w width of the rectangle
/// @param [in] _h height of the rectangle
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void colouredRectangle(
												SDL_Surface *_screen,
												int _x,
												int _y,
												int _w,
												int _h,
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
	SDL_WM_SetCaption( "Clear screen to random colour", 0 );
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
		int x=randomRange(0,WINDOW_WIDTH);
		int y=randomRange(0,WINDOW_HEIGHT);
		int w=randomRange(1,200);
		int h=randomRange(1,200);
		colouredRectangle(screen,x,y,w,h,r,g,b);
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


void colouredRectangle(
												SDL_Surface *_screen,
												int _x,
												int _y,
												int _w,
												int _h,
												char _r,
												char _g,
												char _b
											)
{
	SDL_Rect rect;
	rect.x=_x;
	rect.y=_y;
	rect.w=_w;
	rect.h=_h;
	// first we need to convert the rgb triple into the correct int value
	// this is based on the current video Bits per pixel which we grab
	// from the video surface passed in
	Uint32 colour =SDL_MapRGB(_screen->format,_r,_g,_b);
	// next we fill the screen using the SDL_FillRect function,
	// the 2nd parameter is SDL_Rect *dstrect this is a rectangle to fill
	// if it's set to NULL it indicates the whole screen
	SDL_FillRect( _screen, &rect, colour );
	// finally we need to tell SDL to Flip and update the screen
	SDL_Flip(_screen);

}


