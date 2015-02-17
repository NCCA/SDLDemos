#include "Mat4.h"
#include <cstring> // for memcpy

Mat4::Mat4(float _s)
{
  memset(&m_m,0,sizeof(m_m));
  m_00=_s;
  m_11=_s;
  m_22=_s;
  m_33=_s;
}


Mat4::Mat4(const Mat4 &_r)
{
	memcpy(m_m,&_r.m_m,sizeof(m_m));
}

void Mat4::identity()
{
	memset(&m_m,0,sizeof(m_m));
	m_00=1.0f;
	m_11=1.0f;
	m_22=1.0f;
	m_33=1.0f;
}
