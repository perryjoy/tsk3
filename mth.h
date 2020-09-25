#ifndef __MTH_H_
#define __MTH_H_

#include "MTHDEF.H"
#include "VEC.H"
#include "MATR.H"
#include "COLOR.H"

 /* Base large integer types */
typedef unsigned int UINT;
typedef unsigned long long UINT64;
typedef long long INT64;

typedef mth::COLOR<unsigned char> COLOR;
typedef mth::VEC2<float> VEC2f;
typedef mth::VEC2<int> VEC2i;
typedef mth::VEC2<double> VEC;
typedef mth::VEC<double> VEC3;
typedef mth::MATR<double> MATR;

#endif // __MTH_H_
