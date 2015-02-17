/* a basic window in SDL use the command line
g++ -Wall -g ClearScreen.cpp -o ClearScreen `sdl2-config --cflags --libs`

or

clang++ -Wall -g ClearScreen.cpp -o ClearScreen `sdl2-config --cflags --libs`


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
/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg);



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
	win = SDL_CreateWindow("A Basic SDL Window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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


	SDL_Event event;
	bool quit=false;
	//-----------------------------------------------------------------------------
	// now we loop until the quit flag is set to true
	//-----------------------------------------------------------------------------
	while(!quit)
	{


		// process SDL events, in this case we are looking for keys
		while ( SDL_PollEvent(&event) )
		{
			switch (event.type)
			{
				//-----------------------------------------------------------------------------
				// this is the window x being clicked.
				//-----------------------------------------------------------------------------
				case SDL_QUIT : quit = true; break;

				//-----------------------------------------------------------------------------
				// now we look for a keydown event
				//-----------------------------------------------------------------------------
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

		//-----------------------------------------------------------------------------
		// now sleep so colour change isn't too fast
		//-----------------------------------------------------------------------------
		sleep(1);
	} // end processing loop

	//-----------------------------------------------------------------------------
	// finally when we are done we need to tidy up SDL by calling SDL_Quit
	// sometime this is added as the atexit function to make it happen
	// automatically
	//-----------------------------------------------------------------------------
	SDL_Quit();

	return EXIT_SUCCESS;
}



//-----------------------------------------------------------------------------
void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
