#ifndef SDL_CANVAS_H__
#define SDL_CANVAS_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

class SDLCanvas
{
  public :
    static SDLCanvas * instance();
    inline void setColour(char _r, char _g, char _b)
    {m_r=_r; m_g=_g; m_b=_b;}
    inline void setBackgroundColour(char _r, char _g, char _b)
    {m_bgR=_r; m_bgG=_g; m_bgB=_b;}
    /// @brief simple SDL clear screen function
    void clearScreen();
    /// @brief flip the screen
    inline void flip()const {SDL_Flip(m_screen);}
    /// @brief set the screen dimensions etc
    void setScreen(int _w, int _h);
    inline int width()const {return m_width;}
    inline int height()const {return m_height;}
    inline void setPointSize(int _s){m_pointSize=_s;}

    /// @brief simple SDL clear screen function
    /// @param [in] _screen a pointer to the SDL screen structure
    /// @param [in] _x position of the rectangle
    /// @param [in] _y position of the rectangle
    /// @param [in] _r the red component for the clear colour 0-255
    /// @param [in] _g the green component for the clear colour 0-255
    /// @param [in] _b the blue component for the clear colour 0-255
    void putPixel(
                  int _x,
                  int _y
                );

    /// @brief Bresenham's line algorithm from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
    /// @param [in] _x0 the first x position
    /// @param [in] _y0 the first y position
    /// @param [in] _radius the radius of the circle
    /// @param [in] _r the red component for the clear colour 0-255
    /// @param [in] _g the green component for the clear colour 0-255
    /// @param [in] _b the blue component for the clear colour 0-255
    void drawCircle(
                    int _x0,
                    int _y0,
                    int _radius
                   );
    /// @brief return a random value in the range _min -> _max
    /// @param [in] _min the min value
    /// @param [in] _max the max value
    /// @returns a random value from _min -> _max

    /// @brief Bresenham's line algorithm from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
    /// @param [in] _x0 the first x position
    /// @param [in] _y0 the first y position
    /// @param [in] _x1 the second x position
    /// @param [in] _y1 the second x position
    void drawLine(
                  int _x0,
                  int _y0,
                  int _x1,
                  int _y1
                 );

    void renderText(int _x, int _y, const std::string &_c);

  private :

    SDLCanvas();
    ~SDLCanvas();
    SDLCanvas(SDLCanvas &_c);
    static SDLCanvas* m_pinstance;

    char m_r;
    char m_g;
    char m_b;
    char m_bgR;
    char m_bgG;
    char m_bgB;
    int m_pointSize;
    SDL_Surface *m_screen;
    int m_width;
    int m_height;
    TTF_Font *m_font;

};




#endif
