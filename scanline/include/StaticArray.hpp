#ifndef STATIC_ARRAY_INCLUDED
#define STATIC_ARRAY_INCLUDED

#include <assert.h>

template<typename T, unsigned int Size>
class StaticArray
{
public :
	StaticArray() {}
	StaticArray(const T& val) { clear(val); }
	StaticArray(const StaticArray& cpy) { for(unsigned int i=0;i<Size;i++) m_data[i]=cpy.m_data[i]; }

	void clear(const T& val) { for(unsigned int i=0;i<Size;i++) m_data[i]=val; }

	unsigned int capacity() const { return Size; }
	unsigned int size() const { return Size; }

	const T& operator[](unsigned int i) const { assert(i<size()); return m_data[i]; }
	T& operator[](unsigned int i) { assert(i<size()); return m_data[i]; }

	const T* begin() const { return &m_data[0]; }
    const T* end() const { return &m_data[Size]; }
	T* begin() { return &m_data[0]; }
    T* end() { return &m_data[Size]; }

	StaticArray<T,Size> operator+(const StaticArray<T,Size>& rhs) const
	{
		StaticArray<T,Size> ret;
		for(unsigned int i=0;i<Size;i++) 
		{
			ret[0]=m_data[i]+rhs[i];
		}
		return ret;
	}
	StaticArray<T,Size> operator-(const StaticArray<T,Size>& rhs) const
	{
		StaticArray<T,Size> ret;
		for(unsigned int i=0;i<Size;i++) 
		{
			ret[0]=m_data[i]-rhs[i];
		}
		return ret;
	}
	StaticArray<T,Size> operator*(float mult) const
	{
		StaticArray<T,Size> ret;
		for(unsigned int i=0;i<Size;i++) 
		{
			ret[0]=m_data[i]*mult;
		}
		return ret;
	}

private :
	T m_data[Size];
};

#endif // STATIC_ARRAY_INCLUDED
