/* a basic window in SDL use the command line
g++ -Wall -g DrawLine.cpp -o DrawLine `sdl-config --cflags --libs`

or

clang++ -Wall -g DrawLine.cpp -o DrawLine `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL.h>
#include <cstdlib>
#include <iostream>
//-----------------------------------------------------------------------------
/// @brief the width of the window
//-----------------------------------------------------------------------------
const int WINDOW_WIDTH = 1024;
//-----------------------------------------------------------------------------
/// @brief the height of the window
//-----------------------------------------------------------------------------
const int WINDOW_HEIGHT = 720;

//-----------------------------------------------------------------------------
/// @brief simple SDL clear screen function
/// @param [in] _screen a pointer to the SDL screen structure
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
//-----------------------------------------------------------------------------
void clearScreen(SDL_Renderer *_ren,char _r,char _g,char _b);

//-----------------------------------------------------------------------------
/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg);
//-----------------------------------------------------------------------------
/// @brief return a random value in the range _min -> _max
/// @param [in] _min the min value
/// @param [in] _max the max value
/// @returns a random value from _min -> _max
//-----------------------------------------------------------------------------

int randomRange(int _min,int _max	);

void drawLine(SDL_Renderer *_renderer,int _x0,int _y0,int _x1,int _y1,char _r,	char _g,char _b );


int main()
{
	//-----------------------------------------------------------------------------
	// First thing we need to do is initialise SDL in this case we are
	// setting up just the video subsystem if we need audio or timer etc
	// we would | (or) the flags together see http://www.libsdl.org/intro.en/usinginit.html
	// we check the return value and if not 0 it is an error
	//-----------------------------------------------------------------------------
	if (SDL_Init( SDL_INIT_VIDEO ) !=0)
	{
		SDLErrorExit("error initialising SDL");
	}
	//-----------------------------------------------------------------------------
	// next we create a window and make sure it works
	//-----------------------------------------------------------------------------
	SDL_Window *win = 0;
	win = SDL_CreateWindow("Bresenham's Line Drawing Algorithm", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == 0)
	{
			SDLErrorExit("Error creating Window");
	}
	//-----------------------------------------------------------------------------
	// now we associate a renderer with the window
	//-----------------------------------------------------------------------------
	SDL_Renderer *renderer = 0;
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == 0)
	{
		SDLErrorExit("error creating renderer");
	}
	clearScreen(renderer,0,0,0);

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
		int y0=randomRange(0,WINDOW_WIDTH);
		int x1=randomRange(0,WINDOW_WIDTH);
		int y1=randomRange(0,WINDOW_HEIGHT);

		drawLine(renderer,x0,y0,x1,y1,r,g,b);

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


int randomRange(int _min,int _max	)
{
	return (int)((double)rand() / ((double)RAND_MAX + _min) * _max);
}




//-----------------------------------------------------------------------------
void clearScreen(SDL_Renderer *_ren,char _r,char _g,char _b	)
{
	SDL_SetRenderDrawColor(_ren, _r,_g,_b,255);

	SDL_RenderClear(_ren);
	SDL_RenderPresent(_ren);
}

//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}



// modified from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void drawLine(SDL_Renderer *_renderer,int _x0,int _y0,int _x1,int _y1,char _r,	char _g,char _b )
{
	SDL_SetRenderDrawColor(_renderer, _r,_g,_b,255);
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
			SDL_RenderDrawPoint(_renderer,y,x);

		}
		else
		{
			SDL_RenderDrawPoint(_renderer,x,y);

		}
		error-=deltay;
		if(error <0)
		{
			y+=ystep;
			error+=deltax;
		}
	}
	SDL_RenderPresent(_renderer);
}


