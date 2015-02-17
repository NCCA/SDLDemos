#include <SDL.h>
#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

#include <iostream>

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
void InitOpenGLDisplay(int Width,int Height);



#ifdef DARWIN
	int SDL_main(int argc, char *argv[])
#else
	int main(int argc, char *argv[])
#endif
{

  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }



	// Create our opengl context and attach it to our window
	InitOpenGLDisplay(1024,720);

  // now clear the screen and swap whilst NGL inits (which may take time)
  glClear(GL_COLOR_BUFFER_BIT);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;
  // now we create an instance of our ngl class, this will init NGL and setup basic
  // opengl stuff ext. When this falls out of scope the dtor will be called and cleanup
  // our gl stuff
  while(!quit)
  {

    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_w : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_s : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events


    glClear(GL_COLOR_BUFFER_BIT);
    // now draw etc

    // swap the buffers
    SDL_GL_SwapBuffers();

  }
  // now tidy up and exit SDL
 SDL_Quit();
}


void InitOpenGLDisplay(int Width,int Height)
{
	const SDL_VideoInfo *info=NULL;
	info = SDL_GetVideoInfo();
	int bpp = info->vfmt->BitsPerPixel;
	int flags=0;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	flags = SDL_OPENGL; //|SDL_FULLSCREEN;
	if(SDL_SetVideoMode(Width,Height,bpp,flags)==0)
	{
		SDLErrorExit("Video Mode Failed ");
	}

	glEnable(GL_DEPTH_TEST);
	// set the clear colour
	glClearColor(0.4,0.4,0.4,1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
