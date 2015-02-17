#include "GLFunctions.h"
#include <iostream>
#include "Vec4.h"
#include "Mat4.h"
#include <cmath>

void GLFunctions::cube(GLfloat _w, GLfloat _h, GLfloat _d)
{
  // assume the  cube centered at the origin
  GLfloat w=_w/2.0f;
  GLfloat h=_h/2.0f;
  GLfloat d=_d/2.0f;


    glBegin(GL_QUADS);
   // front face
    glNormal3f(0,0,1);
    glVertex3f(-w,h,d);
    glVertex3f(w,h,d);
    glVertex3f(w,-h,d);
    glVertex3f(-w,-h,d);
    // back face
    glNormal3d(0,0,-1);
    glVertex3f(-w,h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,-h,-d);
    glVertex3f(-w,-h,-d);
    // Left face
    glNormal3f(1,0,0);
    glVertex3f(-w,-h,d);
    glVertex3f(-w,-h,-d);
    glVertex3f(-w,h,-d);
    glVertex3f(-w,h,d);
    // Right face
    glNormal3f(-1,0,0);
    glVertex3f(w,-h,d);
    glVertex3f(w,-h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,h,d);
   // Top face
    glNormal3f(0,1,0);
    glVertex3f(-w,h,d);
    glVertex3f(-w,h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,h,d);
    // Bottom face
    glNormal3f(0,-1,0);
   glVertex3f(-w,-h,d);
   glVertex3f(-w,-h,-d);
   glVertex3f(w,-h,-d);
   glVertex3f(w,-h,d);

  glEnd();
}

void GLFunctions::lookAt(Vec4 _eye, Vec4 _look, Vec4 _up)
{
  Vec4 n =   _look-_eye;
   Vec4 u = _up;
   Vec4 v = n.cross(u);
   u = v.cross(n);
   n.normalize();
   v.normalize();
   u.normalize();

   Mat4 mv(1.0f);
   mv.m_00= v.m_x;
   mv.m_10= v.m_y;
   mv.m_20= v.m_z;
   mv.m_01= u.m_x;
   mv.m_11= u.m_y;
   mv.m_21= u.m_z;
   mv.m_02=-n.m_x;
   mv.m_12=-n.m_y;
   mv.m_22=-n.m_z;
   mv.m_30=-_eye.dot(v);
   mv.m_31=-_eye.dot(u);
   mv.m_32= _eye.dot(n);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMultMatrixf(&mv.m_openGL[0]);
}


void GLFunctions::perspective(float _fovy,float _aspect, float _zNear, float _zFar)
{
  float range = tan(radians(_fovy / 2.0)) * _zNear;
  float left = -range * _aspect;
  float right = range * _aspect;
  float bottom = -range;
  float top = range;
  Mat4 result;
  result.m_00 = (2.0f * _zNear) / (right - left);
  result.m_11 = (2.0f * _zNear) / (top - bottom);
  result.m_22 = - (_zFar + _zNear) / (_zFar - _zNear);
  result.m_23 = - 1.0f;
  result.m_32 = - (2.0f* _zFar * _zNear) / (_zFar - _zNear);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(&result.m_openGL[0]);

}

 float GLFunctions::radians(float _deg )
{
  return (_deg/180.0f) * M_PI;
}



 void GLFunctions::sphere(float _radius, int _precision )
 {
   //  Sphere code based on a function Written by Paul Bourke.
   //  http://astronomy.swin.edu.au/~pbourke/opengl/sphere/
   // the next part of the code calculates the P,N,UV of the sphere for tri_strips

   float theta1 = 0.0;
   float theta2 = 0.0;
   float theta3 = 0.0;
   float nx,ny,nz,u,v,x,y,z;
   glBegin(GL_TRIANGLE_STRIP);

   if( _radius < 0 )
     {
         _radius = -_radius;
     }
     // Disallow a negative number for _precision.
     if( _precision < 4 )
     {
         _precision = 4;
     }
   // now fill in a vertData structure and add to the data list for our sphere
   for( int i = 0; i < _precision/2; ++i )
   {
     theta1 = i * TWO_PI / _precision - PI2;
     theta2 = (i + 1) * TWO_PI / _precision - PI2;

     for( int j = 0; j <= _precision; ++j )
     {
         theta3 = j * TWO_PI / _precision;

         nx = cosf(theta2) * cosf(theta3);
         ny = sinf(theta2);
         nz = cosf(theta2) * sinf(theta3);
         x = _radius * nx;
         y = _radius * ny;
         z = _radius * nz;

         u  = (j/(float)_precision);
         v  = 2*(i+1)/(float)_precision;
         glNormal3f(nx,ny,nz);
         glTexCoord2d(u,v);
         glVertex3d(x,y,z);

         nx = cosf(theta1) * cosf(theta3);
         ny = sinf(theta1);
         nz = cosf(theta1) * sinf(theta3);
         x = _radius * nx;
         y = _radius * ny;
         z = _radius * nz;

         u  = (j/(float)_precision);
         v  = 2*i/(float)_precision;
         glNormal3f(nx,ny,nz);
         glTexCoord2d(u,v);
         glVertex3d(x,y,z);
       } // end inner loop
   }// end outer loop
   glEnd();
   // end citation
 }



 void GLFunctions::capsule(float _radius,  float _height,   int _precision  )

 {
   // based on code from here
   // https://code.google.com/p/rgine/source/browse/trunk/RGine/opengl/src/RGLShapes.cpp
   /// @todo add UV's at some stage
   glBegin(GL_TRIANGLES);
   float x,y,z,nx,ny,nz;
   float h = _height / 2;
    float s, c, s1, c1, o;
    float sb, cb, sb1, cb1;
    float ang = (1.0 / _precision) * (M_PI);
    for (int i = 0; i < 2 * _precision; ++i)
    {

      c = _radius * cos(ang * i);
      c1= _radius * cos(ang * (i + 1));
      s = _radius * sin(ang * i);
      s1 =_radius * sin(ang * (i + 1));
      //side top
      x = c1;
      y = h;
      z = s1;

      nx=c1;
      ny= 0.0;
      nz=s1;
      glNormal3d(nx,ny,nz);
      glVertex3f(x,y,z);
       x = c;
       z = s;
       nx=c;
       nz=s;
       glNormal3d(nx,ny,nz);
       glVertex3f(x,y,z);
      y = -h;
      glNormal3d(nx,ny,nz);
      glVertex3f(x,y,z);

      //side bot
      glNormal3d(nx,ny,nz);
      glVertex3f(x,y,z);
      x = c1;
      z = s1;
      nx= c1;
      nz=s1;
      glNormal3d(nx,ny,nz);
      glVertex3f(x,y,z);
      x = c1;
      y = h;
      z = s1;
      nx=c1;
      nz=s1;
      glNormal3d(nx,ny,nz);
      glVertex3f(x,y,z);

      for (int j = 0; j <=_precision; ++j)
      {
        if (j < _precision / 2)
        {
          o = h;
        }
        else
        {
          o = -h;
        }
        //longitude
        s = -sin(ang * i);
        s1 = -sin(ang * (i + 1));
        c = cos(ang * i);
        c1 = cos(ang * (i + 1));
        //latitude
        sb = _radius * sin(ang * j);
        sb1 = _radius * sin(ang * (j + 1));
        cb = _radius * cos(ang * j);
        cb1 = _radius * cos(ang * (j + 1));
        if (j != _precision - 1)
        {
          nx=x = sb * c;
          ny=y = cb + o;
          nz=z = sb * s;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);

          nx=x = sb1 * c;
          ny=y = cb1 + o;
          nz=z = sb1 * s;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);
          nx=x = sb1 * c1;
          ny=y = cb1 + o;
          nz=z = sb1 * s1;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);
        }
        if (j != 0)
        {
          nx=x = sb * c;
          ny=y = cb + o;
          nz=z = sb * s;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);
          nx=x = sb1 * c1;
          ny=y = cb1 + o;
          nz=z = sb1 * s1;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);
          nx=x = sb * c1;
          ny=y = cb + o;
          nz=z = sb * s1;
          glNormal3d(nx,ny,nz);
          glVertex3f(x,y,z);
        }
      }
    }
    glEnd();
    // end citation

 }



 /*----------------------------------------------------------------------------------------------------------------------
  * Compute lookup table of cos and sin values forming a cirle
  * borrowed from free glut implimentation of primitive drawing
  *
  * Notes:
  *    It is the responsibility of the caller to free these tables
  *    The size of the table is (n+1) to form a connected loop
  *    The last entry is exactly the same as the first
  *    The sign of n can be flipped to get the reverse loop
  */
 //----------------------------------------------------------------------------------------------------------------------
/// borrowed from glut source
 void GLFunctions::fghCircleTable(double **io_sint, double **io_cost, const int _n  )
 {
   int i;
   /* Table size, the sign of n flips the circle direction */
   const int size = abs(_n);
   /* Determine the angle between samples */
   const double angle = 2*M_PI/(double)( ( _n == 0 ) ? 1 : _n );

   /* Allocate memory for n samples, plus duplicate of first entry at the end */
   *io_sint = new double[size+1];
   *io_cost = new double[size+1];

   /* Compute cos and sin around the circle */
   (*io_sint)[0] = 0.0;
   (*io_cost)[0] = 1.0;

   for (i=1; i<size; ++i)
   {
     (*io_sint)[i] = sin(angle*i);
     (*io_cost)[i] = cos(angle*i);
   }
   /* Last sample is duplicate of the first */
   (*io_sint)[size] = (*io_sint)[0];
   (*io_cost)[size] = (*io_cost)[0];
 }
// end citation

 void GLFunctions::cylinder( float _radius,const float _height, int _slices, int _stacks )
 {
   /* Step in z and radius as stacks are drawn. */

   double z0,z1;
   const double zStep = _height / ( ( _stacks > 0 ) ? _stacks : 1 );

   /* Pre-computed circle */

   double *sint,*cost;

   fghCircleTable(&sint,&cost,-_slices);
   float x,y,z,nx,ny,nz,s,t;
   /* Do the stacks */
   z0 = 0.0;
   z1 = zStep;
   // texture co-ords start at 0,0
   // texture steps
   float du=1.0/_stacks;
   float dv=1.0/_slices;
   /* Cover each stack with a quad strip, except the top stack */
   float u=0.0;
   float v=0.0;
   glBegin(GL_TRIANGLES);
   for(int i=1; i<=_stacks+1; ++i )
   {
     if(i==_stacks)
     {
         z1 = _height;
     }
     for(int j=0; j<=_slices-1; ++j)
     {
       // vert 1;
       s=u;
       t=v;
       nx=sint[j];
       ny=cost[j];
       nz=0;
       x=sint[j]*_radius;
       y=cost[j]*_radius;
       z=-z0/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);
       // vert 2
       t=v+dv;
       z=-z1/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);
       // vert 3
       s=u+du;
       t=v;
       nx=sint[j+1];
       ny=cost[j+1];
       x=sint[j+1]*_radius;
       y=cost[j+1]*_radius;
       z=-z0/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);


       // vert 1;
       s=u+du;
       t=v;
       nx=sint[j+1];
       ny=cost[j+1];
       nz=0;
       x=sint[j+1]*_radius;
       y=cost[j+1]*_radius;
       z=-z0/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);

       // vert 1;
       s=u;
       t=v+dv;
       nx=sint[j];
       ny=cost[j];
       nz=0;
       x=sint[j]*_radius;
       y=cost[j]*_radius;
       z=-z1/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);

       // vert 1;
       s=u+du;
       t=v+dv;
       nx=sint[j+1];
       ny=cost[j+1];
       nz=0;
       x=sint[j+1]*_radius;
       y=cost[j+1]*_radius;
       z=-z1/2.0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);

       u+=du;
     }
     v+=dv;
     u=0.0;
     z0 = z1; z1 += zStep;
    }
   glEnd();
     /* Release sin and cos tables */

     delete [] sint;
     delete [] cost;
 }

 void GLFunctions::cone(float _base,float _height,  int _slices, int _stacks  )
 {
     /* Step in z and radius as stacks are drawn. */
     double z0,z1;
     double r0,r1;

     const double zStep = _height / ( ( _stacks > 0 ) ? _stacks : 1 );
     const double rStep = _base / ( ( _stacks > 0 ) ? _stacks : 1 );

     /* Scaling factors for vertex normals */

   //	const double cosn = ( _height / sqrt ( _height * _height + _base * _base ));
   //	const double sinn = ( _base   / sqrt ( _height * _height + _base * _base ));

     /* Pre-computed circle */
     glBegin(GL_TRIANGLE_STRIP);
     double *sint,*cost;
     fghCircleTable(&sint,&cost,-_slices);
     z0 = 0.0;
     z1 = zStep;

     r0 = _base;
     r1 = r0 - rStep;
     // texture co-ords start at 0,0
     // texture steps
     float du=1.0/_stacks;
     float dv=1.0/_slices;
     /* Cover each stack with a quad strip, except the top stack */
     float u=1.0;
     float v=1.0;

     float x,y,z,nx,ny,nz,s,t;
   float phi = (float)atan(_base/_height);
   float sphi= (float)sin(phi);


   for(int i=0; i<_stacks; i++ )
   {
     for(int j=0; j<=_slices; j++)
     {
       s=u;
       t=v;

       float theta = j == _slices ? 0.f : (float) j / _slices * TWO_PI;
       float ctheta = (float)cos(theta);
       float stheta = (float)sin(theta);

       nx = ctheta;
       ny = -stheta;
       nz = sphi;

       x=cost[j]*r0;
       y=sint[j]*r0;
       z=z0;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);
       // now for the next verts
       s=u;
       t=v-dv;

       x=cost[j]*r1;
       y=sint[j]*r1;
       z=z1;
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);
       u-=du;
     }
     v-=dv;
     u=1.0;
     z0 = z1; z1 += zStep;
     r0 = r1; r1 -= rStep;

   }
   // create VAO
   glEnd();
   /* Release sin and cos tables */

   delete [] sint;
   delete [] cost;
 }


 void GLFunctions::disk(float _radius,  int _slices )
 {
     /* Pre-computed circle */
     double *sint,*cost;

     fghCircleTable(&sint,&cost,-_slices);
     // as were using a triangle fan its  vert at the center then
     // the points
     float x,y,z,nx,ny,nz,s,t;
     // texture co-ords start at 0,0
     // texture steps
     float du=1.0/_slices;

     float u=0.0;
     float v=0.0;
     glBegin(GL_TRIANGLE_FAN);
     // as we are doing a tri fan this is the center
     s=u;
     t=v;
     nx=0.0;
     ny=0.0;
     nz=-1.0;
     x=0;
     y=0;
     z=0;
     glTexCoord2f(s,t);
     glNormal3f(nx,ny,nz);
     glVertex3f(x,y,z);
     v=1.0;

     for (int j=0; j<=_slices; ++j)
     {
         s=u;
         t=v;
         // normals set above
         x=cost[j]*_radius;
         y=sint[j]*_radius;
         // z set above
         glTexCoord2f(s,t);
         glNormal3f(nx,ny,nz);
         glVertex3f(x,y,z);
         u+=du;
     }
     glEnd();
     /* Release sin and cos tables */
     delete [] sint;
     delete [] cost;
 }



 void GLFunctions::torus( float _minorRadius, float _majorRadius,int _nSides, int _nRings,const bool _flipTX )
 {
     float  iradius = _minorRadius, oradius = _majorRadius, phi, psi, dpsi, dphi;

     float spsi, cpsi, sphi, cphi ;

     if ( _nSides < 1 ) { _nSides = 1; }
     if ( _nRings < 1 ) { _nRings = 1; }

     /* Increment the number of sides and rings to allow for one more point than surface */
     _nSides ++ ;
     _nRings ++ ;
     float *vertex= new float[3 * _nSides * _nRings];
     float *normal= new float[3 * _nSides * _nRings];
     float *uv= new float[2*_nSides*_nRings*2];

     dpsi =  2.0 * M_PI / (double)(_nRings - 1) ;
     dphi = -2.0 * M_PI / (double)(_nSides - 1) ;
     psi  = 0.0;
     float uStep=1.0/_nRings;
     float vStep=1.0/_nSides;
     float tu=0.0;
     float tv=0.0;
     // pre compute the values for the torus
     for(int j=0; j<_nRings; ++j )
     {
       cpsi = cos ( psi ) ;
       spsi = sin ( psi ) ;
       phi = 0.0;
       for(int i=0; i<_nSides; ++i )
       {
         int offset = 3 * ( j * _nSides + i ) ;
         cphi = cos ( phi ) ;
         sphi = sin ( phi ) ;
         *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
         *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
         *(vertex + offset + 2) =                    sphi * iradius  ;
         *(normal + offset + 0) = cpsi * cphi ;
         *(normal + offset + 1) = spsi * cphi ;
         *(normal + offset + 2) =        sphi ;
         if (_flipTX)
         {
           *(uv+offset)=tv;
           *(uv+offset+1)=tu;
         }
         else
         {
           *(uv+offset)=tu;
           *(uv+offset+1)=tv;
         }
         tu+=uStep;
         phi += dphi;
         } // end of _nSides loop
         tu=0.0;
         tv+=vStep;
         psi += dpsi;
       }  // end of _nRings loop

     float *n,*v,*tt;
     float x,y,z,nx,ny,nz,s,t;
     glBegin(GL_TRIANGLES);
     for(int i=0; i<_nSides-1; ++i )
     {
         for(int j=0; j<_nRings-1; ++j )
         {
             int offset = 3 * ( j * _nSides + i ) ;
             n=normal+offset;
             v=vertex+offset;
             tt=uv+offset;
             s=*tt;
             t=*(tt+1);
             nx=*n;
             ny=*(n+1);
             nz=*(n+2);
             x=*v;
             y=*(v+1);
             z=*(v+2);
       //V1
             glTexCoord2f(s,t);
             glNormal3f(nx,ny,nz);
             glVertex3f(x,y,z);
             // vert
             n=normal+offset+3;
             v=vertex+offset+3;
             tt=uv+offset+3;
             s=*tt;
             t=*(tt+1);
             nx=*n;
             ny=*(n+1);
             nz=*(n+2);
             x=*v;
             y=*(v+1);
             z=*(v+2);
       //V2
             glTexCoord2f(s,t);
             glNormal3f(nx,ny,nz);
             glVertex3f(x,y,z);

             n=normal + offset + 3 * _nSides + 3;
             v=vertex + offset + 3 * _nSides + 3;
             tt=uv+ offset+3*_nSides+3;
             // next vert
             s=*tt;
             t=*(tt+1);
             nx=*n;
             ny=*(n+1);
             nz=*(n+2);
             x=*v;
             y=*(v+1);
             z=*(v+2);
       //V3
             glTexCoord2f(s,t);
             glNormal3f(nx,ny,nz);
             glVertex3f(x,y,z);


             n=normal+offset;
             v=vertex+offset;
             tt=uv+offset;
             s=*tt;
             t=*(tt+1);
             nx=*n;
             ny=*(n+1);
             nz=*(n+2);
             x=*v;
             y=*(v+1);
             z=*(v+2);
             //V1
             glTexCoord2f(s,t);
             glNormal3f(nx,ny,nz);
             glVertex3f(x,y,z);


       n=normal + offset + 3 * _nSides + 3;
       v=vertex + offset + 3 * _nSides + 3;
       tt=uv+ offset+3*_nSides+3;
       // next vert
       s=*tt;
       t=*(tt+1);
       nx=*n;
       ny=*(n+1);
       nz=*(n+2);
       x=*v;
       y=*(v+1);
       z=*(v+2);
       //V3
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);

       n=normal + offset + 3 * _nSides;
       v= vertex + offset + 3 * _nSides;
       tt= uv+offset+3*_nSides;
       // next vert
       s=*tt;
       t=*(tt+1);
       nx=*n;
       ny=*(n+1);
       nz=*(n+2);
       x=*v;
       y=*(v+1);
       z=*(v+2);
       glTexCoord2f(s,t);
       glNormal3f(nx,ny,nz);
       glVertex3f(x,y,z);

         } // end _nRings
     } // end _nSides
     delete [] vertex;
     delete [] normal;
     delete [] uv;

     // now create the VBO
     glEnd();
 }

