#ifndef __MATR_H_
#define __MATR_H_

#include "MTHDEF.H"
#include "VEC.H"

namespace mth {
template<class Type>
struct MATR {
   Type m[4][4];                       /* matrix array */

  // typedef VEC<Type> VEC;

   explicit MATR(VOID)
   {
      m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
      m[0][1] = m[0][2] = m[0][3] = m[1][0] = 0;
      m[1][2] = m[1][3] = m[2][0] = m[2][1] = 0;
      m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0;

   }

   Type GetElement(INT i, INT j)
   {
      return m[i][j];
   }

   MATR operator*(const MATR & a) const
   {
      INT k;
      MATR r;

      for (INT i = 0; i < 4; i++)
         for (INT j = 0; j < 4; j++)
            for (k = 0, r.m[i][j] = 0; k < 4; k++)
               r.m[i][j] += m[i][k] * a.m[k][j];
      return r;
   }

   MATR & operator=(const MATR & a)
   {
      for (INT i = 0; i < 4; i++)
         for (INT j = 0; j < 4; j++)
            m[i][j] = a.m[i][j];
      return *this;
   }

   operator Type * (VOID)
   {
      return &(m[0][0]);
   }

   MATR &operator*=(const MATR & a)
   {
      *this = *this * a;
      return *this;
   }

   static Type MatrDet3x3(Type a11, Type a12, Type a13,
      Type a21, Type a22, Type a23,
      Type a31, Type a32, Type a33)
   {
      return a11 * a22 *a33 - a11 * a23 * a32 - a12 * a21 * a33 +
         a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;
   }

   Type MatrDeterm(VOID) const
   {
      return m[0][0] * MatrDet3x3(m[1][1], m[1][2], m[1][3],
         m[2][1], m[2][2], m[2][3],
         m[3][1], m[3][2], m[3][3]) -
         m[0][1] * MatrDet3x3(m[1][0], m[1][2], m[1][3],
            m[2][0], m[2][2], m[2][3],
            m[3][0], m[3][2], m[3][3]) +
         m[0][2] * MatrDet3x3(m[1][0], m[1][1], m[1][3],
            m[2][0], m[2][1], m[2][3],
            m[3][0], m[3][1], m[3][3]) -
         m[0][3] * MatrDet3x3(m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2],
            m[3][0], m[3][1], m[3][2]);
   }

   MATR CalcInverse(VOID) const
   {
      MATR r, in;
      INT p[4][3] = { {1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2} };
      Type d = MatrDeterm();


      for (INT i = 0; i < 4; i++)
         for (INT j = 0; j < 4; j++)
            r.m[j][i] = MATR::MatrDet3x3(
               m[p[i][0]][p[j][0]], m[p[i][0]][p[j][1]], m[p[i][0]][p[j][2]],
               m[p[i][1]][p[j][0]], m[p[i][1]][p[j][1]], m[p[i][1]][p[j][2]],
               m[p[i][2]][p[j][0]], m[p[i][2]][p[j][1]], m[p[i][2]][p[j][2]]) / d;


      return r;
   }

   MATR MatrTranslate(const VEC<Type> & t) const
   {
      MATR<Type> r;

      r.m[3][0] = t.x;
      r.m[3][1] = t.y;
      r.m[3][2] = t.z;

      return (*this) * r;
   }

   MATR MatrScale(const VEC<Type> & t) const
   {
      MATR<Type> r;

      r.m[0][0] = t.x;
      r.m[1][1] = t.y;
      r.m[2][2] = t.z;

      return (*this) * r;
   }

   MATR MatrRotateX(DBL angleInDegrees) const
   {
      MATR<Type> r;
      DBL a = VEC<Type>::DegreeToRadian(angleInDegrees);

      r.m[2][2] = r.m[1][1] = cos(a);
      r.m[2][1] = -sin(a);
      r.m[1][2] = -r.m[2][1];

      return (*this) * r;
   }

   MATR MatrRotateY(DBL angleInDegrees) const
   {
      MATR<Type> r;
      DBL a = VEC<Type>::DegreeToRadian(angleInDegrees);

      r.m[2][2] = r.m[0][0] = cos(a);
      r.m[0][2] = -sin(a);
      r.m[2][0] = -r.m[0][2];

      return (*this) * r;
   }

   MATR MatrRotateZ(DBL angleInDegrees) const
   {
      MATR<Type> r;
      DBL a = VEC<Type>::DegreeToRadian(angleInDegrees);

      r.m[0][0] = r.m[1][1] = cos(a);
      r.m[1][0] = -sin(a);
      r.m[0][1] = -r.m[1][0];

      return (*this) * r;
   }

   MATR MatrRotate(DBL angleInDegrees, const VEC<Type> & a) const
   {
      DBL rad = VEC<Type>::DegreeToRadian(angleInDegrees);
      VEC<Type> n = a.Norm();
      DBL si = sin(rad), co = cos(rad), x = n.x, y = n.y, z = n.z;
      MATR<Type> r;

      r.m[0][0] = co + x * x * (1 - co);
      r.m[0][1] = x * y * (1 - co) - z * si;
      r.m[0][2] = x * z * (1 - co) + y * si;
      r.m[1][0] = y * x * (1 - co) + z * si;
      r.m[1][1] = co + y * y * (1 - co);
      r.m[1][2] = y * z * (1 - co) - x * si;
      r.m[2][0] = z * x * (1 - co) - y * si;
      r.m[2][1] = z * y * (1 - co) + x * si;
      r.m[2][2] = co + z * z * (1 - co);

      return (*this) * r;
   }

   MATR MatrTranspose(VOID) const
   {
      MATR<Type> r;
      for (INT i = 0; i < 4; i++)
         for (INT j = 0; j < 4; j++)
            r.m[i][j] = m[j][i];
      return r;
   }

   template<class VecType>
   static MATR MatrView(const VEC<VecType> & loc, const VEC<VecType> & at, const VEC<VecType> & up1)
   {
      VEC<VecType> dir = (at - loc).Norm(),
         right = Cross(dir, up1).Norm(),
         up = Cross(right, dir).Norm();
      VecType rx = right.x, ry = right.y, rz = right.z,
         ux = up.x, uy = up.y, uz = up.z,
         dx = dir.x, dy = dir.y, dz = dir.z,
         lx = loc.x, ly = loc.y, lz = loc.z;
      MATR v;
      v.m[0][0] = rx, v.m[0][1] = ux, v.m[0][2] = -dx, v.m[0][3] = 0;
      v.m[1][0] = ry, v.m[1][1] = uy, v.m[1][2] = -dy, v.m[1][3] = 0;
      v.m[2][0] = rz, v.m[2][1] = uz, v.m[2][2] = -dz, v.m[2][3] = 0;
      v.m[3][0] = -Dot(loc, right);
      v.m[3][1] = -Dot(loc, up);
      v.m[3][2] = Dot(loc, dir);
      v.m[3][3] = 1;
      return v;
   }

   template<class VecType>
   static MATR MatrViewDir(const VEC<VecType> & loc, const VEC<VecType> & dir, const VEC<VecType> & up1)
   {
      VEC<VecType> d = !dir,
         right = !Cross(dir, up1),
         up = !Cross(right, dir);
      VecType rx = right.x, ry = right.y, rz = right.z,
         ux = up.x, uy = up.y, uz = up.z,
         dx = d.x, dy = d.y, dz = d.z,
         lx = loc.x, ly = loc.y, lz = loc.z;
      MATR v;
      v.m[0][0] = rx, v.m[0][1] = ux, v.m[0][2] = -dx, v.m[0][3] = 0;
      v.m[1][0] = ry, v.m[1][1] = uy, v.m[1][2] = -dy, v.m[1][3] = 0;
      v.m[2][0] = rz, v.m[2][1] = uz, v.m[2][2] = -dz, v.m[2][3] = 0;
      v.m[3][0] = -loc & right;
      v.m[3][1] = -loc & up;
      v.m[3][2] = loc & d;
      v.m[3][3] = 1;
      return v;
   }

   template<class Type>
   static MATR MatrFrustum(Type l, Type r, Type b, Type t, Type n, Type f)
   {
      MATR fru;
      fru.m[0][0] = 2 * n / (r - l), fru.m[0][1] = 0, fru.m[0][2] = 0, fru.m[0][3] = 0;
      fru.m[1][0] = 0, fru.m[1][1] = 2 * n / (t - b), fru.m[1][2] = 0, fru.m[1][3] = 0;
      fru.m[2][0] = (r + l) / (r - l), fru.m[2][1] = (t + b) / (t - b), fru.m[2][2] = -(f + n) / (f - n), fru.m[2][3] = -1;
      fru.m[3][0] = 0, fru.m[3][1] = 0, fru.m[3][2] = -2 * n * f / (f - n), fru.m[3][3] = 0;
      return fru;
   }

   static MATR MatrRotation(DBL angleInDegrees, const VEC<Type> & a)
   {
      MATR<Type> r;
      r = r.MatrRotate(angleInDegrees, a);
      return r;
   }

   static MATR MatrRotationX(DBL angleInDegrees)
   {
      MATR<Type> r;
      r = r.MatrRotateX(angleInDegrees);
      return r;
   }

   static MATR MatrRotationY(DBL angleInDegrees)
   {
      MATR<Type> r;
      r = r.MatrRotateY(angleInDegrees);
      return r;
   }

   static MATR MatrRotationZ(DBL angleInDegrees)
   {
      MATR<Type> r;
      r = r.MatrRotateZ(angleInDegrees);
      return r;
   }

   static MATR MatrTranslation(const VEC<Type> & t)
   {
      MATR<Type> r;
      r = r.MatrTranslate(t);
      return r;
   }

   static MATR MatrScaling(const VEC<Type> & t)
   {
      MATR<Type> r;
      r = r.MatrScale(t);
      return r;
   }

};

template<class Type>
std::ostream &operator<<(std::ostream & c, const MATR<Type> & b)
{
   Type a[4][4];
   a = b.ToArray();
   c << a[0][0] << ' ' << a[0][1] << ' ' << a[0][2] << ' ' << a[0][3] << std::endl
      << a[1][0] << ' ' << a[1][1] << ' ' << a[1][2] << ' ' << a[1][3] << std::endl
      << a[2][0] << ' ' << a[2][1] << ' ' << a[2][2] << ' ' << a[2][3] << std::endl
      << a[3][0] << ' ' << a[3][1] << ' ' << a[3][2] << ' ' << a[3][3] << std::endl;
   return c;
}

}
#endif // !__MATR_H_
