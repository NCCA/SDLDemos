#include "Vec4.h"
#include "Mat4.h"
#include "cmath"
#include "cassert"


Vec4::Vec4(const Vec4 &_c)
{
  m_x=_c.m_x;
  m_y=_c.m_y;
  m_z=_c.m_z;
  m_w=_c.m_w;
}


Vec4 Vec4::cross(const Vec4 _r) const
{
	return Vec4(
								m_y*_r.m_z - m_z*_r.m_y,
								m_z*_r.m_x - m_x*_r.m_z,
								m_x*_r.m_y - m_y*_r.m_x,
								0.0
							 );
}
/// @brief dot product of vector
float Vec4::dot(const Vec4 _r) const
{
	return m_x * _r.m_x + m_y * _r.m_y + m_z * _r.m_z;
}


float Vec4::length() const
{
  return sqrt(m_x*m_x + m_y*m_y + m_z * m_z);
}

void Vec4::normalize()
{
  float l=length();
  assert(l !=0.0);
  m_x/=l;
  m_y/=l;
  m_z/=l;
}

Vec4 Vec4::operator -(const Vec4 &_r) const
{
  return Vec4(m_x-_r.m_x, m_y-_r.m_y,m_z-_r.m_z,m_w);
}
Vec4 Vec4::operator +(const Vec4 &_r) const
{
  return Vec4(m_x+_r.m_x, m_y+_r.m_y,m_z+_r.m_z,m_w);

}
void Vec4::operator +=(const Vec4 &_r)
{
  m_x+=_r.m_x;
  m_y+=_r.m_y;
  m_z+=_r.m_z;
}

void Vec4::operator -=(const Vec4 &_r)
{
  m_x-=_r.m_x;
  m_y-=_r.m_y;
  m_z-=_r.m_z;
}

bool Vec4::operator ==(const Vec4 &_r) const
{
 const static float delta=0.001;
 return ( ((m_x)-delta)<(_r.m_x) && ((m_x)+delta)>(_r.m_x) ) &&
        ( ((m_y)-delta)<(_r.m_y) && ((m_y)+delta)>(_r.m_y) ) &&
        ( ((m_z)-delta)<(_r.m_z) && ((m_z)+delta)>(_r.m_z) );
}
float Vec4::operator [](int _i) const
{
	assert(_i >=0 && _i<=3);
	return (&m_x)[_i];
}

Vec4 Vec4::operator *(const Mat4 &_r) const
{
	Vec4 (
					 m_x*_r.m_00 + m_y*_r.m_10 + m_z*_r.m_20+ m_w*_r.m_30,
					 m_x*_r.m_01 + m_y*_r.m_11 + m_z*_r.m_21+ m_w*_r.m_31,
					 m_x*_r.m_02 + m_y*_r.m_12 + m_z*_r.m_22+ m_w*_r.m_32,
					 m_x*_r.m_03 + m_y*_r.m_13 + m_z*_r.m_23+ m_w*_r.m_33
					);
}
Vec4 Vec4::operator *(float _r) const
{
	return Vec4(m_x*_r,m_y*_r,m_z*_r,m_w);
}
void Vec4::operator *=(float _r)
{
	m_x*=_r;
	m_y*=_r;
	m_z*=_r;

}

