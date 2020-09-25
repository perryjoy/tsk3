#ifndef __COLOR_H_
#define __COLOR_H_

#include "MTHDEF.H"

namespace mth {
template <class Type>
struct COLOR {
   Type b, g, r, a;

   COLOR(Type red, Type green, Type blue, Type alpha) : r(red), g(green), b(blue), a(alpha)
   {
   }

   COLOR(Type c = 0) : r(c), g(c), b(c), a(1)
   {
   }

   COLOR operator+(const COLOR & v) const
   {
      return COLOR(min((int)r + v.r, 255), min((int)g + v.g, 255), min((int)b + v.b, 255), min((int)a + v.a, 255));
   }

   COLOR operator*(const double & n) const
   {
      return COLOR(r * n, g * n, b * n, a);
   }

   operator Type *(VOID)
   {
      return &r;
   }
};
}
#endif // __COLOR_H_
