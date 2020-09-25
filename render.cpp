#include "win.h"
#include "Render.h"

VOID RENDER::StartFrame(VOID)
{
   memset(&frame[0], 0, sizeof(DWORD) * frame.size());
}

VOID RENDER::EndFrame(VOID)
{
   HDC hDC = GetDC(hWin);
   SetBitmapBits(hBackBitmap, h * w * sizeof(DWORD), ( const void * )(frame.data()));
   BitBlt(hDC, 0, 0, w, h, hBackDC, 0, 0, SRCCOPY);
   ReleaseDC(hWin, hDC);
}

VOID RENDER::Resize(INT w, INT h)
{
}

VOID RENDER::PutPixel(int x, int y, FLT z, COLOR c)
{
   if (x > 0 && x < w && y > 0 && y < h) {
      COLOR c0 = COLOR(0);// *(( COLOR * )& frame[( size_t )y * w + x]);
      COLOR rez = c0 * (c.a / 255.0) + c * (1.0 - c.a / 255.0);
      frame[( size_t )y * w + x] = *(( DWORD * )& rez);// *(1 - c.a) + frame[( size_t )y * w + x] * c.a;
   }
}

VOID RENDER::Rect(int x1, int y1, int x2, int y2, COLOR c)
{
   for (int y = y1; y < y2; y++)
      for (int x = x1; x < x2; x++)
         PutPixel(x, y, 0, c);
}

VOID RENDER::Bresenham(VEC p0, VEC p1, COLOR c)
{
   INT x0 = (p0.x + 1.0) * w / 2.0, y0 = (-p0.y + 1.0) * h / 2.0, x1 = (p1.x + 1.0) * w / 2.0, y1 = (-p1.y + 1.0) * h / 2.0;
   INT signY = (y1 - y0 > 0) ? 1 : -1, signX = (x1 - x0 > 0) ? 1 : -1;
   FLT d = (fabs(y0 - y1) + 1) / (fabs(x0 - x1) + 1), err = 0;
   if (d < 1) {
      INT y = y0;
      for (INT x = x0; x != x1; x += signX) {
         PutPixel(x, y, -1, c); //  (32, 32, 255, 0)
         err += d;
         if (err > 1) {
            y += signY;
            err--;
         }
      }
   }
   else {
      d = 1.0 / d;
      INT x = x0;
      for (INT y = y0; y != y1; y += signY) {
         PutPixel(x, y, -1, c);
         err += d;
         if (err > 1) {
            x += signX;
            err--;
         }
      }
   }
}

bool IsLeftTurn(VEC prev, VEC cur, VEC next)
{
   return ((DBL)cur.x - prev.x) * ((DBL)next.y - prev.y) - ((DBL)next.x - prev.x) * ((DBL)cur.y - prev.y) >= 0.0;
}

RENDER::RENDER(WIN * window) : hWin(window->hWnd)
{
   w = window->w;
   h = window->h;
   isSized = FALSE;
   frame.resize((size_t)w * h);
   
   HDC hWinDC = GetDC(hWin);

   BITMAPINFO bmi = { 0 };
   bmi.bmiHeader.biSize = sizeof(BITMAPCOREHEADER);
   bmi.bmiHeader.biWidth = w;
   bmi.bmiHeader.biHeight = -h;
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biBitCount = 32;
   bmi.bmiHeader.biCompression = BI_RGB;

   hBackDC = CreateCompatibleDC(hWinDC);
   hBackBitmap = CreateCompatibleBitmap(hWinDC, w, h);
   SetBitmapBits(hBackBitmap, h * w * sizeof(DWORD), ( const void * )(frame.data()));

   SelectObject(hBackDC, hBackBitmap);
   ReleaseDC(hWin, hWinDC);
} 