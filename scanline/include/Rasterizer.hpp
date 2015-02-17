#ifndef ASD_SCANLINE_RASTERIZER_INCLUDED
#define ASD_SCANLINE_RASTERIZER_INCLUDED

#include "StaticArray.hpp"
#include "PixelBuffer.hpp"

typedef StaticArray<float,3> Point3d;
typedef StaticArray<float,3> RGBColour;
typedef StaticArray<int,2> ScreenPosition;

struct Vertex
{
	Point3d position;
	RGBColour colour;
};

class Rasterizer
{
public :
	Rasterizer();
	Rasterizer(unsigned int w, unsigned int h);

	void drawPoint(const Vertex& v);
	void drawLine(const Vertex& start, const Vertex& end);
	void drawTriangle(const Vertex& A, const Vertex& B, const Vertex& C);

	void flush();

	void setDimensions(unsigned int width, unsigned int height);
	unsigned int width() const;
	unsigned int height() const;

	const PixelBuffer3f& getColourBuffer() const { return m_screen; }
private :
	bool normalizedToScreenCoordinates(const Point3d& i_normalized, ScreenPosition* o_screenpos) const;
	void setPixel(unsigned int x, unsigned int y, const RGBColour& col);
	void rasterCore(Vertex vparams[],int iparams[]);
private :
	float m_fdimensions[2];
	PixelBuffer3f m_screen;
};

#endif // ASD_SCANLINE_RASTERIZER_INCLUDED