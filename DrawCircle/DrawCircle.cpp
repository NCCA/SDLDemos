/* a basic window in SDL use the command line
g++ -Wall -g DrawCircle.cpp -o DrawCircle `sdl-config --cflags --libs`

or

clang++ DrawCircle.cpp -o DrawCircle `sdl-config --cflags --libs`

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

/// @brief // from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
/// @param [in] _x0 the first x position
/// @param [in] _y0 the first y position
/// @param [in] _radius the radius of the circle
/// @param [in] _r the red component for the clear colour 0-255
/// @param [in] _g the green component for the clear colour 0-255
/// @param [in] _b the blue component for the clear colour 0-255
void drawCircle(SDL_Renderer *_renderer,int _x0,int _y0,int _radius,char _r,	char _g,char _b );

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
	win = SDL_CreateWindow("Midpoint Circle Algorithm", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool quit=false;
	// now we loop until the quit flag is set to true
	// now we loop until the quit flag is set to true
	while(!quit)
	{
		// generate random colour values from 0-255
		char r=randomRange(0,255);
		char g=randomRange(0,255);
		char b=randomRange(0,255);
		int x0=randomRange(0,WINDOW_WIDTH);
		int y0=randomRange(0,WINDOW_WIDTH);
		int radius=randomRange(0,500);

		drawCircle(renderer,x0,y0,radius,r,g,b);

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





// from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawCircle(SDL_Renderer *_renderer,int _x0,int _y0,int _radius,char _r,	char _g,char _b )
{
	int f = 1 - _radius;
	int ddF_x = 1;
	int ddF_y = -2 * _radius;
	int x = 0;
	int y = _radius;
	SDL_SetRenderDrawColor(_renderer, _r,_g,_b,255);
	SDL_RenderDrawPoint(_renderer,_x0, _y0 + _radius);
	SDL_RenderDrawPoint(_renderer,_x0, _y0 - _radius);
	SDL_RenderDrawPoint(_renderer,_x0 + _radius, _y0);
	SDL_RenderDrawPoint(_renderer,_x0 - _radius, _y0);


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
		SDL_RenderDrawPoint(_renderer,_x0 + x, _y0 + y);
		SDL_RenderDrawPoint(_renderer,_x0 - x, _y0 + y);
		SDL_RenderDrawPoint(_renderer,_x0 + x, _y0 - y);
		SDL_RenderDrawPoint(_renderer,_x0 - x, _y0 - y);
		SDL_RenderDrawPoint(_renderer,_x0 + y, _y0 + x);
		SDL_RenderDrawPoint(_renderer,_x0 - y, _y0 + x);
		SDL_RenderDrawPoint(_renderer,_x0 + y, _y0 - x);
		SDL_RenderDrawPoint(_renderer,_x0 - y, _y0 - x);
	}
	SDL_RenderPresent(_renderer);
}

