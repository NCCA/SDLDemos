/* a basic window in SDL use the command line
g++ -Wall -g CosSine.cpp -o CosSine `sdl-config --cflags --libs`

or

clang++ CosSine.cpp -o CosSine `sdl-config --cflags --libs`

to compile the program
*/

#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <SDL/SDL_ttf.h>
#include "SDLCanvas.h"


/// @brief the width of the window
const int WINDOW_WIDTH = 1080;
/// @brief the height of the window
const int WINDOW_HEIGHT = 800;



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
  SDLCanvas *canvas=SDLCanvas::instance();
  canvas->setScreen(WINDOW_WIDTH,WINDOW_HEIGHT);
	// next we set the window bar caption to the text 2nd param is for an icon
	// this is a char * to a pixmap data but if we use 0 none is loaded
	SDL_WM_SetCaption( "Visualisation of Cos and Sin", 0 );
	// clear screen to white
  canvas->setBackgroundColour(255,255,255);
  canvas->setPointSize(3);
	SDL_Event event;

	bool quit=false;
  static int cx=WINDOW_WIDTH/2;
  static int cy=240;
  bool animate=true;
  std::vector<int> cosTable(360,-10);//cx);
  std::vector<int> sinTable(360,-10);//cy);
	// now we loop until the quit flag is set to true
	int r=200;
	while(!quit)
	{
    static int angle=0;
    float x = r*cos(angle*(M_PI/180.0)) + cx;
    float y = r*sin(angle*(M_PI/180.0)) + cy;

    canvas->clearScreen();
    canvas->setColour(0,0,0);
    char text[200];
    sprintf(text,"Angle %00d degrees %0.02f radians",angle,(angle*(M_PI/180.0)));
    canvas->renderText(10,10,text);
    sprintf(text,"x=%0d*cos(%d*(M_PI/180.0)) = %00.2f",r,angle,x);
    canvas->renderText(10,30,text);
    sprintf(text,"y=%0d*sin(%d*(M_PI/180.0)) = %00.2f",r,angle,y);
    canvas->renderText(10,50,text);
    canvas->drawCircle(cx,cy,r);
		// axis
    canvas->setPointSize(1);

    canvas->drawLine(cx,cy-r,cx,WINDOW_HEIGHT);
    canvas->drawLine(cx-r,cy,WINDOW_WIDTH,cy);


    canvas->setPointSize(3);

		cosTable.insert(cosTable.begin(),int(x));
		sinTable.insert(sinTable.begin(),int(y));
		cosTable.pop_back();
		sinTable.pop_back();
    if(animate )
      angle++;
    if(angle == 360 )
			angle=0;
    canvas->setColour(255,255,0);
    canvas->drawLine(x,y,cx,cy);
		// draw the sine line
    canvas->setColour(0,255,0);
    canvas->drawLine(x,cy,x,y);
		// draw the cos line
    canvas->setColour(255,0,0);
    canvas->drawLine(x,cy,cx,cy);
    canvas->setColour(0,0,0);
    canvas->putPixel(cx,cy);
		// the current point on the circle
    canvas->setColour(255,255,255);
    canvas->putPixel(x,y);

		for(int i=0; i<360; ++i)
		{
      canvas->setColour(0,255,0);
      canvas->putPixel(cx+r+i,sinTable[i]);
      canvas->setColour(255,0,0);
      canvas->putPixel(cosTable[i],cy+r+i);


		}


		// now flip
    canvas->flip();
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
            case SDLK_SPACE : animate^=true; break;
            case SDLK_EQUALS : angle+=1; break;
            case SDLK_MINUS : angle-=1; break;

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


