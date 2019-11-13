/* a basic window in SDL use the command line
g++ -Wall -g  PutPixel.cpp -o PutPixel `sdl-config --cflags --libs`

or

clang++ -Wall -g PutPixel.cpp -o PutPixel `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL.h>
#include <cstdlib>
#include <iostream>
//-----------------------------------------------------------------------------
/// @brief the width of the window
//-----------------------------------------------------------------------------
constexpr size_t WINDOW_WIDTH = 600;
//-----------------------------------------------------------------------------
/// @brief the height of the window
//-----------------------------------------------------------------------------
constexpr size_t WINDOW_HEIGHT = 600;


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
  SDL_Window *win = nullptr;
  win = SDL_CreateWindow("Texture Blit", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
	{
			SDLErrorExit("Error creating Window");
	}
	//-----------------------------------------------------------------------------
	// now we associate a renderer with the window
	//-----------------------------------------------------------------------------
  SDL_Renderer *renderer = nullptr;
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr)
	{
		SDLErrorExit("error creating renderer");
	}
  std::unique_ptr<unsigned int[]> tx=std::make_unique<unsigned int [] >(WINDOW_WIDTH*WINDOW_HEIGHT);
  size_t i=0;
  unsigned int pixel;
  auto setRGBA=[&pixel](unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) noexcept
  {
    pixel=uint32_t(_a | (_b << 8) | (_g << 16) | (_r << 24));
  };
  setRGBA(255,0,0,255);

  for(i=0; i<WINDOW_WIDTH*200; ++i)
    tx[i]=pixel;
  setRGBA(0,255,0,255);

  for(; i<WINDOW_WIDTH*400; ++i)
    tx[i]=pixel;
  setRGBA(0,0,255,255);
  for(; i<WINDOW_WIDTH*600; ++i)
    tx[i]=pixel;

  // create an empty texture
  auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_UpdateTexture(texture,nullptr,tx.get(),WINDOW_WIDTH*sizeof(unsigned int));
  SDL_Event event;
	bool quit=false;
	// now we loop until the quit flag is set to true
	while(!quit)
	{
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

		// finally we need to tell SDL to update the screen
		SDL_RenderPresent(renderer);

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

		SDL_Delay(1000);
	} // end processing loop

	// finally when we are done we need to tidy up SDL by calling SDL_Quit
	// sometime this is added as the atexit function to make it happen
	// automatically
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
