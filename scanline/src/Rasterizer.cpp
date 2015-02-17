#include "Rasterizer.hpp"
#include <math.h>
#include <algorithm>

template<typename T>
void lerp(const T& a, const T& b, float t, T* o_out)
{
	*o_out=a+(b-a)*t;
}


Rasterizer::Rasterizer()
{
	setDimensions(800,600);
}
Rasterizer::Rasterizer(unsigned int w, unsigned int h)
{
	setDimensions(w,h);
}

void Rasterizer::drawPoint(const Vertex& v)
{
	ScreenPosition sp;
	if(normalizedToScreenCoordinates(v.position,&sp))
	{
		setPixel(sp[0],sp[1],v.colour);
	}
}

void Rasterizer::drawLine(const Vertex& start, const Vertex& end)
{
	ScreenPosition p[2];
	normalizedToScreenCoordinates(start.position,&p[0]);
	normalizedToScreenCoordinates(end.position,&p[1]);
	
	bool steep = abs(p[1][1]-p[0][1])>abs(p[1][0]-p[0][0]);
	if (steep) 
	{
		std::swap(p[0][0],p[0][1]);
		std::swap(p[1][0],p[1][1]);
	}

	int exclude=0;
	if (p[0][0] > p[1][0]) 
	{
		std::swap(p[0],p[1]);
		exclude=1;
	}

	int dx = p[1][0]-p[0][0];
	int dy = abs(p[1][1]-p[0][1]);
	int pk=2*dy-dx;
	int ystep;
	p[0][1]<p[1][1] ? ystep=1 : ystep = -1;

	int y=p[0][1];
	for (int x=p[0][0];x<p[1][0];x++)
	{
		RGBColour value;
		lerp(start.colour,end.colour,static_cast<float>(x-p[0][0])/static_cast<float>(p[1][0]-p[0][0]),&value);
		if (steep) 
		{
			setPixel(x,y,value);
		} 
		else 
		{
			setPixel(x,y,value);
		}
		pk+=2*dy;
		if (pk>=0) 
		{
			pk-=2*dx;
			y+=ystep;
		}
	}
}

void Rasterizer::drawTriangle(const Vertex& A, const Vertex& B, const Vertex& C)
{	
}

bool Rasterizer::normalizedToScreenCoordinates(const Point3d& i_normalized, ScreenPosition* o_screenpos) const
{
	if (fabs(std::max(i_normalized[0],i_normalized[1]))>1.f)
		return false;

	// -1<=x<=+1
	// 0<=x+1<=2
	// 0<=(x+1)/2<=1
	// 0<=((x+1)/2)*width<=width
	// Here there is a chance that it reaches exactly width
	// so we clamp (upper)

	(*o_screenpos)[0] = static_cast<unsigned int>(std::min(m_fdimensions[0]-1.f,((i_normalized[0]+1.f)*0.5f)*m_fdimensions[0]));
	(*o_screenpos)[1] = static_cast<unsigned int>(std::min(m_fdimensions[1]-1.f,((i_normalized[1]+1.f)*0.5f)*m_fdimensions[1]));

	return true;
}

void Rasterizer::setDimensions(unsigned int width, unsigned int height)
{
	RGBColour defval(0.f);
	m_screen.resize(width,height,defval);
	m_fdimensions[0]=static_cast<float>(width);
	m_fdimensions[1]=static_cast<float>(height);
}

unsigned int Rasterizer::width() const
{
	return m_screen.width();
}

unsigned int Rasterizer::height() const
{
	return m_screen.height();
}

void Rasterizer::setPixel(unsigned int x, unsigned int y, const RGBColour& col)
{
	m_screen(x,y)=col;
}