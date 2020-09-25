#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")


class RENDER {
   INT w, h;            // window size
   HDC hDC;
   HWND &hWin;
   BOOL isSized;

   HDC hBackDC = NULL;
   HBITMAP hBackBitmap = NULL;
   BITMAPINFO bmi = { 0 };

   std::vector<DWORD> frame;

public:
   RENDER(WIN *window);
   VOID StartFrame(VOID);
   VOID PutPixel(int x, int y, FLT z, COLOR c);
   VOID Rect(int x1, int y1, int x2, int y2, COLOR c);
   VOID EndFrame(VOID);
   VOID Resize(INT w, INT h);
   VOID Bresenham(VEC p0, VEC p1, COLOR c);
};

#endif // __RENDER_H_
