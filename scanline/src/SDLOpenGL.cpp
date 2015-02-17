/* Simple immediate mode OpenGL and SDL
Linux build use
g++ -Wall -g SDLOpenGL.cpp -o SDLOpenGL `sdl-config --cflags --libs` -DLINUX -lGL
clang++ -Wall -g SDLOpenGL.cpp -o SDLOpenGL `sdl-config --cflags --libs` -DLINUX -lGL
for OSX use
clang++ -Wall -g SDLOpenGL.cpp -o SDLOpenGL `sdl-config --cflags --libs` -DDARWIN -framework OpenGL

to compile the program
*/

#include <SDL/SDL.h>
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

#include <cstdlib>
#include <iostream>

#include "Rasterizer.hpp"

/// @brief the width of the window
const int WINDOW_WIDTH = 1024;
/// @brief the height of the window
const int WINDOW_HEIGHT = 720;

/// @brief a function to set the OpenGL display
/// @param[in] _width the width of the window
/// @param[in] _height the height of the window
void initOpenGLDisplay(
												int _width,
												int _height
											);

/// @brief called once to initialise some of the OpenGL state machine like
/// clear colour
void initializeGL();
/// @brief called each frame to draw some random points
void paintGL(const PixelBuffer3f& buffer);
/// @brief generate a random float in the range min->max
/// @param[in] _min the min value
/// @param[in] _max the max value
/// @returns _ a random float in the range min->max
float randFloatRange(
											float _min,
											float _max
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
	// we must setup OpenGL before we call anything else
	initOpenGLDisplay(WINDOW_WIDTH,WINDOW_HEIGHT);
	// now setup the basic gl stuff (clear colour)
	initializeGL();
	// next we set the window bar caption to the text 2nd param is for an icon
	// this is a char * to a pixmap data but if we use 0 none is loaded
	SDL_WM_SetCaption( "SDL and OpenGL", 0 );

    Rasterizer r(WINDOW_WIDTH,WINDOW_HEIGHT);


    Vertex a,b;
    a.position[0]=a.position[1]=a.position[2]=-1.f;
    b.position[0]=b.position[1]=b.position[2]=+1.f;

    a.colour[0]=b.colour[2]=1.f;
    a.colour[1]=a.colour[2]=b.colour[1]=b.colour[0]=0.f;

    r.drawLine(a,b);

	SDL_Event event;
	bool quit=false;
	// now we loop until the quit flag is set to true
	while(!quit)
	{
		// draw our dots
        paintGL(r.getColourBuffer());
		// sleep for a while
		//SDL_Delay(100);
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



void initOpenGLDisplay(int _width,int _height)
{
	const SDL_VideoInfo *info=NULL;
	// get the current video mode
	info = SDL_GetVideoInfo();
	// grab the bits per pixel data
	int bpp = info->vfmt->BitsPerPixel;
	// now set some OpenGL attributes
	// first the min number of bits for the channels
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	// now the min size of the depth buffer (linux is max 24 mac will do 32)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
	// enable double buffering for better animation
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	// now attempt to set the video mode
	int flags = SDL_OPENGL | SDL_HWSURFACE ;
	if(SDL_SetVideoMode(_width,_height,bpp,flags)==0)
	{
		std::cerr<<"Video Mode Failed\n"<<SDL_GetError();
		exit(EXIT_FAILURE);
	}

}

void initializeGL()
{
	//set the clear colour
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
  // set the point size so we can see it
  glPointSize(4);
  glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
}

void paintGL(const PixelBuffer3f& buffer)
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT);
  // start drawing some random points
  //glBegin(GL_POINTS);

  glDrawPixels(WINDOW_WIDTH,WINDOW_HEIGHT,GL_RGB,GL_FLOAT,buffer.pointer());

  // tell OpenGL we have finished with the points
  //glEnd();
  // now tell SDL we need to re-draw
  SDL_GL_SwapBuffers();
}

float randFloatRange(
											float _min,
											float _max
										)
{
	return ((_max-_min)*((float)rand()/RAND_MAX))+_min;
}
