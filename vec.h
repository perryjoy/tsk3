#ifndef __VEC_H_
#define __VEC_H_

#include <iostream>
#include "MTHDEF.H"

namespace mth {
template<class TypeM>
struct MATR;                             /* Forward declarations */

template<class Type>
struct VEC2 {
   Type x, y;

   VEC2(Type x, Type y) : x(x), y(y)
   {
   }

   VEC2(Type c = 0) : x(c), y(c)
   {
   }

   DBL VecLen(VOID) const
   {
      return sqrt(x * x + y * y);
   }

   VEC2 operator!(VOID) const
   {
      DBL l = this->VecLen();
      if (!l)
         l = 1;
      return VEC2(x / l, y / l);
   }

   VEC2 operator-(VOID) const
   {
      return VEC2(-x, -y);
   }

   VEC2 operator+(const VEC2 & v) const
   {
      return VEC2(x + v.x, y + v.y);
   }

   VEC2 operator-(const VEC2 & v) const
   {
      return VEC2(x - v.x, y - v.y);
   }

   VEC2 operator*(DBL n) const
   {
      return VEC2(x * n, y * n);
   }

   VEC2 operator/(DBL n) const
   {
      return VEC2(x / n, y / n);
   }

   static DBL AngleVecPlane(const VEC2 & v1, const VEC2 & v2)
   {
      VEC2<Type> t1 = !v1, t2 = !v2;
      DBL c = acos((t1 & t2) / (t1.VecLen() * t2.VecLen()));

      return (t1.x * t2.y - t2.x * t1.y >= 0) ? c : -c;
   }

   Type operator&(const VEC2 & v) const
   {
      return x * v.x + y * v.y;
   }

   VEC2 &operator=(const VEC2 & v)
   {
      x = v.x;
      y = v.y;
      return *this;
   }

   VEC2 &operator+=(const VEC2 & v)
   {
      *this = *this + v;
      return *this;
   }

   VEC2 &operator-=(const VEC2 & v)
   {
      *this = *this - v;
      return *this;
   }

   VEC2 &operator*=(DBL n)
   {
      *this = *this * n;
      return *this;
   }
};

template<class Type>
struct VEC {
   Type x, y, z;

   VEC(Type x, Type y, Type z) : x(x), y(y), z(z)
   {
   }

   VEC(Type c = 0) : x(c), y(c), z(c)
   {
   }

   DBL VecLen(VOID) const
   {
      return sqrt(x * x + y * y + z * z);
   }

   static DBL DegreeToRadian(DBL a)
   {
      return a * PI / 180;
   }

   static DBL RadianToDegree(DBL r)
   {
      return r * 180 / PI;
   }

   static DBL AngleVec(const VEC & v1, const VEC & v2)
   {
      VEC<Type> t1 = !v1, t2 = !v2;
      DBL c = acos((t1 & t2) / (t1.VecLen() * t2.VecLen()));

      return (t1.x * t2.y - t2.x * t1.y >= 0) ? c : -c;
   }

   bool operator==(const VEC &v) const
   {
      return x == v.x && y == v.y && z == v.z;
   }

   operator Type * (VOID)
   {
      return &x;
   }

   Type &operator[](UINT index)
   {
      switch (index) {
      case 1:
         return y;
      case 2:
         return z;
      default:
         return x;
      }
   }

   VEC operator-(VOID) const
   {
      return VEC(-x, -y, -z);
   }

   VEC operator+(const VEC & v) const
   {
      return VEC(x + v.x, y + v.y, z + v.z);
   }

   VEC operator-(const VEC & v) const
   {
      return VEC(x - v.x, y - v.y, z - v.z);
   }

   VEC operator*(DBL n) const
   {
      return VEC(x * n, y * n, z * n);
   }

   VEC operator/(DBL n) const
   {
      return VEC(x / n, y / n, z / n);
   }

   VEC Norm(VOID) const
   {
      DBL l = VecLen();
      if (!l)
         l = 1;
      return VEC(x / l, y / l, z / l);
   }

   VEC &Normalize(VOID)
   {
      *this = Norm();
      return *this;
   }

   VEC &operator=(const VEC & v)
   {
      x = v.x;
      y = v.y;
      z = v.z;
      return *this;
   }

   VEC &operator+=(const VEC & v)
   {
      *this = *this + v;
      return *this;
   }

   VEC &operator-=(const VEC & v)
   {
      *this = *this - v;
      return *this;
   }

   VEC &operator*=(DBL n)
   {
      *this = *this * n;
      return *this;
   }

   VEC Min(const VEC & v) const
   {
      return VEC((x < v.x) ? x : v.x,
         (y < v.y) ? y : v.y,
         (z < v.z) ? z : v.z);
   }

   VEC Max(const VEC & v) const
   {
      return VEC((x > v.x) ? x : v.x,
         (y > v.y) ? y : v.y,
         (z > v.z) ? z : v.z);
   }

   static VEC Rnd(VOID)
   {
      return VEC(rand(), rand(), rand());
   }

   VEC PointTransform(const MATR<Type> & a) const
   {
      Type w = x * a.m[0][3] + y * a.m[1][3] + z * a.m[2][3] + a.m[3][3];

      return VEC((x * a.m[0][0] + y * a.m[1][0] + z * a.m[2][0] + a.m[3][0]) / w,
         (x * a.m[0][1] + y * a.m[1][1] + z * a.m[2][1] + a.m[3][1]) / w,
         (x * a.m[0][2] + y * a.m[1][2] + z * a.m[2][2] + a.m[3][2]) / w);
   }

   VEC VectorTransform(const MATR<Type> & a) const
   {
      Type p[4][4];

      return VEC<Type>(x * a.m[0][0] + y * a.m[1][0] + z * a.m[2][0],
         x * a.m[0][1] + y * a.m[1][1] + z * a.m[2][1],
         x * a.m[0][2] + y * a.m[1][2] + z * a.m[2][2]);
   }

   VEC operator*(const MATR<Type> & a) const
   {
      return PointTransform(a);
   }
};

template<class Type>
Type Dot(const VEC<Type> & v1, const VEC<Type> & v2)
{
   return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<class Type>
VEC<Type> Cross(const VEC<Type> & v1, const VEC<Type> & v2)
{
   return VEC<Type>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template<class Type>
std::ostream &operator<<(std::ostream & c, const VEC<Type> & v)
{
   c << '(' << v.x() << ',' << v.y() << ',' << v.z << ')';
   return c;
}
}

#endif // __VEC_H_
