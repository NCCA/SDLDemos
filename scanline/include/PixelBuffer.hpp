#ifndef ASD_PIXELBUFFER_INCLUDED
#define ASD_PIXELBUFFER_INCLUDED

#include <vector>
#include "StaticArray.hpp"

template<typename T, unsigned int nElements>
class PixelBuffer
{
public :
	typedef StaticArray<T,nElements> Pixel;
	typedef PixelBuffer<T,nElements> thisType;
public :
	PixelBuffer();
	PixelBuffer(const thisType& cpy);
	thisType& operator=(const thisType& i_cpy);
	~PixelBuffer();

	PixelBuffer(unsigned int i_width, unsigned int i_height);
	PixelBuffer(unsigned int i_width, float i_aspectratio);

	unsigned int numComponents() const { return nElements; }

	unsigned int width() const;
	unsigned int height() const;
	float aspect_ratio() const;

	const Pixel& operator()(unsigned int x, unsigned int y) const;
	Pixel& operator()(unsigned int x, unsigned int y);

	void clear(const Pixel& i_value);
	void resize(unsigned int i_width, unsigned int i_height, const Pixel& i_value);

    const T* pointer() const;
private:
	struct pimpl;
	pimpl* m_data;
};

typedef PixelBuffer<float,1> PixelBuffer1f;
typedef PixelBuffer<float,2> PixelBuffer2f;
typedef PixelBuffer<float,3> PixelBuffer3f;
typedef PixelBuffer<float,4> PixelBuffer4f;
typedef PixelBuffer<double,1> PixelBuffer1d;
typedef PixelBuffer<double,2> PixelBuffer2d;
typedef PixelBuffer<double,3> PixelBuffer3d;
typedef PixelBuffer<double,4> PixelBuffer4d;


#endif //ASD_PIXELBUFFER_INCLUDED
