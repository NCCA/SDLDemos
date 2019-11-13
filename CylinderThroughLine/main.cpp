#include <SDL.h>
#include <string>
#include <ngl/Vec3.h>
#include <ngl/Util.h>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <ngl/NGLStream.h>
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
	ngl::Vec3 from(2,3,0);
	ngl::Vec3 to(0,2,1);
	ngl::Mat4 persp=ngl::perspective(45,float(1024/720),0.01,500);
	ngl::Mat4  look=ngl::lookAt(ngl::Vec3(4,4,4),ngl::Vec3(0,0,0),ngl::Vec3(0,1,0));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&persp.m_openGL[0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&look.m_openGL[0]);

	glEnable(GL_LIGHT0);
	glColor3f(1,1,0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	float rotx=0.0;
	float roty=0.0;
	float radius=0.4;

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
            case SDLK_o : rotx +=0.5; break;
            case SDLK_p : rotx -=0.5; break;
            case SDLK_5 : radius +=0.1; break;
            case SDLK_6 : radius -=0.1; break;

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
    glDisable(GL_LIGHTING);

    glPushMatrix();
      glRotatef(rotx,1,0,0);
      glRotatef(rotx,0,1,0);
      glLineWidth(4);
      glBegin(GL_LINES);
        glColor3f(1.0f,0,0);
        glVertex3fv(&from.m_openGL[0]);
        glColor3f(0,1.0f,0);
        glVertex3fv(&to.m_openGL[0]);
      glEnd();
      glLineWidth(2);
      ngl::Vec3 p1p2= to-from;
      ngl::Vec3 p;
      p.m_x = rand(); /* Create a random vector */
      p.m_y = rand();
      p.m_z = rand();
      ngl::Vec3 r=p1p2.cross(p);
      ngl::Vec3 s=p1p2.cross(r);
      //p1p2.normalize();
      r.normalize();
      s.normalize();

      glBegin(GL_LINES);
        glColor3f(1.0f,0,0);
        glVertex3fv(&from.m_openGL[0]);
        glVertex3fv(&p1p2.m_openGL[0]);
        glColor3f(0.0,1,0);
        glVertex3fv(&from.m_openGL[0]);
        glVertex3fv(&r.m_openGL[0]);
        glColor3f(0.0,0,1);
        glVertex3fv(&from.m_openGL[0]);
        glVertex3fv(&s.m_openGL[0]);
      glEnd();
    glPopMatrix();
  // code based on this http://paulbourke.net/geometry/circlesphere/
    ngl::Vec3 n,px,py;
    glEnable(GL_LIGHTING);

   double dtheta=ngl::TWO_PI/36;
   for (float t=0.0; t<=1.1; t+=0.1)
   {
     ngl::Vec3 pos=ngl::lerp(from,to,t);
     glBegin(GL_TRIANGLE_FAN);
     glVertex3fv(&pos.m_openGL[0]);
     for (float theta=0;theta<2*M_PI;theta+=dtheta)
     {
          n.m_x = r.m_x * cos(theta) + s.m_x * sin(theta);
          n.m_y = r.m_y * cos(theta) + s.m_y * sin(theta);
          n.m_z = r.m_z * cos(theta) + s.m_z * sin(theta);
          n.normalize();
          glNormal3fv(&n.m_openGL[0]);

          px.m_x = pos.m_x + radius*r.m_x * cos(theta) + radius*s.m_x * sin(theta);
          px.m_y = pos.m_y + radius*r.m_y * cos(theta) + radius*s.m_y * sin(theta);
          px.m_z = pos.m_z + radius*r.m_z * cos(theta) + radius*s.m_z * sin(theta);
          py.m_x = pos.m_x + radius*r.m_x * cos(theta+dtheta) + radius*s.m_x * sin(theta+dtheta);
          py.m_y = pos.m_y + radius*r.m_y * cos(theta+dtheta) + radius*s.m_y * sin(theta+dtheta);
          py.m_z = pos.m_z + radius*r.m_z * cos(theta+dtheta) + radius*s.m_z * sin(theta+dtheta);
          glVertex3fv(&px.m_openGL[0]);
          glVertex3fv(&py.m_openGL[0]);
     }
     glEnd();
   }

    // swap the buffers
    SDL_GL_SwapBuffers();

  }
  // now tidy up and exit SDL
 SDL_Quit();
 return 0;
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
