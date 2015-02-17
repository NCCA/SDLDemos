#ifndef VEC4_H__
#define VEC4_H__

class Mat4;

class Vec4
{
public :

  Vec4(float _x=0.0f, float _y=0.0, float _z=0.0, float _w=1.0f) :
        m_x(_x),m_y(_y),m_z(_z),m_w(_w){;}

  Vec4(const Vec4 &_c);
  /// @brief cross product of vector
  Vec4 cross(const Vec4 _r) const ;
  /// @brief dot product of vector
  float dot(const Vec4 _r) const;

  void normalize();

  float length() const;


  Vec4 operator -(const Vec4 &_r) const;
  Vec4 operator +(const Vec4 &_r) const;
  void operator +=(const Vec4 &_r);
  void operator -=(const Vec4 &_r);
  bool operator ==(const Vec4 &_r) const ;
  float operator [](int _i) const ;
  Vec4 operator *(const Mat4 &_r) const;
  Vec4 operator *(float _r) const;
  void operator *=(float _r);


  #pragma pack(push,1)
    union
    {
      struct
      {
        float m_x;
        float m_y;
        float m_z;
        float m_w;
      };
  #pragma pack(pop)
      float m_openGL[4];
    };


};



#endif
