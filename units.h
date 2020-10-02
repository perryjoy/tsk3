#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"
class ANIM;

class UNIT {
public:
   ANIM *ani = NULL;
   virtual VOID Draw(VOID) = 0;
   virtual VOID Response(VOID) = 0;
};

#endif // __UNITS_H_
