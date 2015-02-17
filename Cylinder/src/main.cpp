#include <SDL.h>
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

#include <iostream>
#include "GLFunctions.h"

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
void InitOpenGLDisplay(int Width,int Height);



int main(int argc, char *argv[])
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
  GLFunctions::perspective(45,float(1024/720),0.01,500);
  GLFunctions::lookAt(Vec4(4,4,4),Vec4(0,0,0),Vec4(0,1,0));
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColor3f(1,1,0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;
  Vec4 from(0,1,0,0);
  Vec4 to(0,2,0,0);
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
            const static float inc=0.1;
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_3 : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_4 : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_w : from.m_y+=inc; break;
            case SDLK_s : from.m_y-=inc; break;
            case SDLK_d : from.m_x+=inc; break;
            case SDLK_a : from.m_x-=inc; break;
            case SDLK_1 : from.m_z+=inc; break;
            case SDLK_2 : from.m_z-=inc; break;

            case SDLK_UP : to.m_y+=inc; break;
            case SDLK_DOWN : to.m_y-=inc; break;
            case SDLK_RIGHT : to.m_x+=inc; break;
            case SDLK_LEFT : to.m_x-=inc; break;
            case SDLK_LEFTBRACKET : to.m_z+=inc; break;
            case SDLK_RIGHTBRACKET : to.m_z-=inc; break;

            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // now draw etc
    glColor3f(1.0f,1.0f,1.0f);
      glBegin(GL_LINES);
        glVertex3fv(&from.m_openGL[0]);
        glVertex3fv(&to.m_openGL[0]);
      glEnd();

      /*


      p1p2 = VectorSub(p1,p2);
      p.x = rand();
      p.y = rand();
      p.z = rand();
      CROSSPROD(p1p2,p,r);
      CROSSPROD(p1p2,r,s);
      Normalise(&r);
      Normalise(&s);
      */

      // calc your y vector
      Vec4 y=from-to;
      // create world up
      Vec4 up(0,1,0,0);
      // check to see if the y vector is not up
      if (y.m_y <0)
        up.set(0,0,1);
      // now take cross product for x axis
      Vec4 x=y.cross(up);
      // now get the local z
      Vec4 z=y.cross(x);
      // normalize
      x.normalize();
      y.normalize();
      z.normalize();
      // set a scale and draw
      float scale=2.0;
//      x*=scale;
//      y*=scale;
//      z*=scale;
//      glBegin(GL_LINES);
//        glColor3f(1,0,0);
//        glVertex3fv(&from.m_openGL[0]);
//        glVertex3fv(&x.m_openGL[0]);
//        glColor3f(0,1,0);
//        glVertex3fv(&from.m_openGL[0]);
//        glVertex3fv(&y.m_openGL[0]);
//        glColor3f(0,0,1);
//        glVertex3fv(&from.m_openGL[0]);
//        glVertex3fv(&z.m_openGL[0]);
//      glEnd();

      double dtheta = 2*M_PI / 36;

      Vec4 n,px,py;
      glBegin(GL_TRIANGLE_FAN);
      glVertex3fv(&from.m_openGL[0]);
      glNormal3fv(&y.m_openGL[0]);

      for (float theta=0;theta<2*M_PI;theta+=dtheta)
      {
           n.m_x = y.m_x * cos(theta) + z.m_x * sin(theta);
           n.m_y = y.m_y * cos(theta) + z.m_y * sin(theta);
           n.m_z = y.m_z * cos(theta) + z.m_z * sin(theta);
           n.normalize();
           px.m_x = from.m_x + y.m_x * cos(theta) + z.m_x * sin(theta);
           px.m_y = from.m_y + y.m_y * cos(theta) + z.m_y * sin(theta);
           px.m_z = from.m_z + y.m_z * cos(theta) + z.m_z * sin(theta);
           py.m_x = from.m_x + y.m_x * cos(theta+dtheta) + z.m_x * sin(theta+dtheta);
           py.m_y = from.m_y + y.m_y * cos(theta+dtheta) + z.m_y * sin(theta+dtheta);
           py.m_z = from.m_z + y.m_z * cos(theta+dtheta) + z.m_z * sin(theta+dtheta);
           glVertex3fv(&px.m_openGL[0]);
           glVertex3fv(&py.m_openGL[0]);


      }
      glEnd();
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


}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
