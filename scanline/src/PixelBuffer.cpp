#include "PixelBuffer.hpp"
#include <assert.h>

template<typename T, unsigned int nElements>
struct PixelBuffer<T, nElements>::pimpl
{
	unsigned int m_dimensions[2];
	std::vector<Pixel> m_data;
};

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>::PixelBuffer() : m_data(0)
{
	m_data = new pimpl;

	if (m_data)
	{
		m_data->m_dimensions[0]=800;
		m_data->m_dimensions[1]=600;

		m_data->m_data.resize(
			m_data->m_dimensions[0]*m_data->m_dimensions[1]);
	}
}

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>::PixelBuffer(const thisType& cpy)
{
	m_data = new pimpl;

	if (m_data)
	{
		m_data->m_dimensions[0]=cpy.m_data->m_dimensions[0];
		m_data->m_dimensions[1]=cpy.m_data->m_dimensions[1];
		m_data->m_data=cpy.m_data->m_data; // Memory copy will be made by std::vector
	}
}

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>& PixelBuffer<T, nElements>::operator=(const thisType& i_cpy)
{
	assert(m_data);
	m_data->m_dimensions[0]=i_cpy.m_data->m_dimensions[0];
	m_data->m_dimensions[1]=i_cpy.m_data->m_dimensions[1];
	m_data->m_data=i_cpy.m_data->m_data;
	return *this;
}

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>::~PixelBuffer()
{
	if (m_data)
		delete m_data;
}

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>::PixelBuffer(unsigned int i_width, unsigned int i_height)
{
	m_data = new pimpl;

	if (m_data)
	{
		m_data->m_dimensions[0]=i_width;
		m_data->m_dimensions[1]=i_height;
		m_data->m_data.resize(i_width*i_height);
	}
}

template<typename T, unsigned int nElements>
PixelBuffer<T, nElements>::PixelBuffer(unsigned int i_width, float i_aspectratio)
{
	m_data = new pimpl;

	if (m_data)
	{
		m_data->m_dimensions[0]=i_width;
		m_data->m_dimensions[1]=static_cast<unsigned int>(
			static_cast<float>(i_width)/i_aspectratio);
		m_data->m_data.resize(m_data->m_dimensions[0]*m_data->m_dimensions[1]);
	}
}

template<typename T, unsigned int nElements>
unsigned int PixelBuffer<T, nElements>::width() const
{
	assert(m_data);
	return m_data->m_dimensions[0];
}

template<typename T, unsigned int nElements>
unsigned int PixelBuffer<T, nElements>::height() const
{
	assert(m_data);
	return m_data->m_dimensions[1];
}

template<typename T, unsigned int nElements>
float PixelBuffer<T, nElements>::aspect_ratio() const
{
	assert(m_data);
	return 
		static_cast<float>(m_data->m_dimensions[0]) /
		static_cast<float>(m_data->m_dimensions[1]);
}

template<typename T, unsigned int nElements>
const typename PixelBuffer<T, nElements>::Pixel&
PixelBuffer<T, nElements>::operator()(unsigned int x, unsigned int y) const
{
	assert(m_data);
	assert(x<m_data->m_dimensions[0]);
	assert(y<m_data->m_dimensions[1]);
	return m_data->m_data[m_data->m_dimensions[0]*y+x];
}

template<typename T, unsigned int nElements>
typename PixelBuffer<T, nElements>::Pixel&
PixelBuffer<T, nElements>::operator()(unsigned int x, unsigned int y)
{
	assert(m_data);
	assert(x<m_data->m_dimensions[0]);
	assert(y<m_data->m_dimensions[1]);
	return m_data->m_data[m_data->m_dimensions[0]*y+x];
}

template<typename T, unsigned int nElements>
void PixelBuffer<T, nElements>::clear(const Pixel& i_value)
{
	assert(m_data);
	m_data->m_data.resize(0);
	m_data->m_data.resize(m_data->m_dimensions[0]*m_data->m_dimensions[1],i_value);
}

template<typename T, unsigned int nElements>
void PixelBuffer<T, nElements>::resize(unsigned int i_width, unsigned int i_height, const Pixel& i_value)
{
	assert(m_data);
	m_data->m_dimensions[0]=i_width;
	m_data->m_dimensions[1]=i_height;
	m_data->m_data.resize(0);
	m_data->m_data.resize(m_data->m_dimensions[0]*m_data->m_dimensions[1],i_value);
}

template<typename T, unsigned int nElements>
const T* PixelBuffer<T, nElements>::pointer() const
{
    return &m_data->m_data[0][0];
}

template class PixelBuffer<float,1>;
template class PixelBuffer<float,2>;
template class PixelBuffer<float,3>;
template class PixelBuffer<float,4>;
template class PixelBuffer<double,1>;
template class PixelBuffer<double,2>;
template class PixelBuffer<double,3>;
template class PixelBuffer<double,4>;
