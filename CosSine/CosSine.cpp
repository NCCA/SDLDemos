/* a basic window in SDL use the command line
g++ -Wall -g CosSine.cpp -o CosSine `sdl-config --cflags --libs`

or

clang++ CosSine.cpp -o CosSine `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
//#include "SDL_ttf.h"


/// @brief the width of the window
const int WINDOW_WIDTH = 1200;
/// @brief the height of the window
const int WINDOW_HEIGHT = 800;

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

/// @brief Bresenham's line algorithm from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
/// @param [in] _x0 the first x position
/// @param [in] _y0 the first y position
/// @param [in] _radius the radius of the circle
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void drawCircle(
								SDL_Surface *_screen,
								int _x0,
								int _y0,
								int _radius,
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

void drawBlock(SDL_Surface *_screen,
							int _x,
							int _y,
							char _r,
							char _g,
							char _b);


/// @brief Bresenham's line algorithm from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
/// @param [in] _x0 the first x position
/// @param [in] _y0 the first y position
/// @param [in] _x1 the second x position
/// @param [in] _y1 the second x position
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void drawLine(
							SDL_Surface *_screen,
							int _x0,
							int _y0,
							int _x1,
							int _y1,
							char _r,
							char _g,
							char _b
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
	SDL_WM_SetCaption( "Visualisation of Cos and Sin", 0 );
	// clear screen to white
	clearScreen(screen,255,255,255);

	SDL_Event event;

	bool quit=false;
	std::vector<int> cosTable;
	std::vector<int> sinTable;
	cosTable.resize(360);
	sinTable.resize(360);
	bool m_animate=false;
	// now we loop until the quit flag is set to true
	int r=200;
	while(!quit)
	{
		clearScreen(screen,255,255,255);
		static int cx=WINDOW_WIDTH/2;
		static int cy=240;
		drawCircle(screen,cx,cy,r,0,0,0);
		// axis
		drawLine(screen,cx,cy+r,cx,WINDOW_HEIGHT,0,0,0);
		drawLine(screen,cx+r,cy,WINDOW_WIDTH,cy,0,0,0);

		static int angle=0;

		float x = r*cos(angle*(M_PI/180.0)) + cx;
		float y = r*sin(angle*(M_PI/180.0)) + cy;
		if(m_animate == true)
		{
			cosTable.insert(cosTable.begin(),int(x));
			sinTable.insert(sinTable.begin(),int(y));
			cosTable.pop_back();
			sinTable.pop_back();
			if(angle++ == 360)
				angle=0;
		}
		drawLine(screen,x,y,cx,cy,255,255,0);
		// draw the sine line
		drawLine(screen,x,cy,x,y,0,255,0);
		// draw the cos line
		drawLine(screen,x,cy,cx,cy,255,0,0);
		drawBlock(screen,cx,cy,0,0,0);
		// the current point on the circle
		drawBlock(screen,x,y,255,255,255);

		for(int i=0; i<360; ++i)
		{
			drawBlock(screen,cx+r+i,sinTable[i],0,255,0);
			drawBlock(screen,cosTable[i],cy+r+i,255,0,0);

		}


		// now flip
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
						case SDLK_r : ++r; break;
						case SDLK_e : --r; break;
						case SDLK_SPACE : m_animate ^=true; break;
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
// from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawCircle(
								SDL_Surface *_screen,
								int _x0,
								int _y0,
								int _radius,
								char _r,
								char _g,
								char _b
							 )
{
	int f = 1 - _radius;
	int ddF_x = 1;
	int ddF_y = -2 * _radius;
	int x = 0;
	int y = _radius;

	putPixel(_screen,_x0, _y0 + _radius,_r,_g,_b);
	putPixel(_screen,_x0, _y0 - _radius,_r,_g,_b);
	putPixel(_screen,_x0 + _radius, _y0,_r,_g,_b);
	putPixel(_screen,_x0 - _radius, _y0,_r,_g,_b);

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
		drawBlock(_screen,_x0 + x, _y0 + y,_r,_g,_b);
		drawBlock(_screen,_x0 - x, _y0 + y,_r,_g,_b);
		drawBlock(_screen,_x0 + x, _y0 - y,_r,_g,_b);
		drawBlock(_screen,_x0 - x, _y0 - y,_r,_g,_b);
		drawBlock(_screen,_x0 + y, _y0 + x,_r,_g,_b);
		drawBlock(_screen,_x0 - y, _y0 + x,_r,_g,_b);
		drawBlock(_screen,_x0 + y, _y0 - x,_r,_g,_b);
		drawBlock(_screen,_x0 - y, _y0 - x,_r,_g,_b);
	}
}


void drawBlock(SDL_Surface *_screen,
							int _x,
							int _y,
							char _r,
							char _g,
							char _b)
{
	putPixel(_screen,_x,_y,_r,_g,_b);
	putPixel(_screen,_x,_y-1,_r,_g,_b);
	putPixel(_screen,_x,_y+1,_r,_g,_b);
	putPixel(_screen,_x+1,_y,_r,_g,_b);
	putPixel(_screen,_x-1,_y,_r,_g,_b);

	putPixel(_screen,_x+1,_y-1,_r,_g,_b);
	putPixel(_screen,_x-1,_y-1,_r,_g,_b);

	putPixel(_screen,_x+1,_y+1,_r,_g,_b);
	putPixel(_screen,_x-1,_y+1,_r,_g,_b);

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
			drawBlock(_screen,y,x,_r,_g,_b);
		}
		else
		{
			drawBlock(_screen,x,y,_r,_g,_b);
		}
		error-=deltay;
		if(error <0)
		{
			y+=ystep;
			error+=deltax;
		}
	}
}


