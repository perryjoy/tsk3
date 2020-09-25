#ifndef __WIN_H_
#define __WIN_H_

#include "def.h"

#define MAX_UNITS 30

#define NUM_OF_KEYS 256

enum MOUSE_BUTTONS {
   LEFT = 0,
   RIGHT = 1,
   MIDDLE = 2
};

struct MOUSE {
   mth::VEC2<int> pos, delta;
   BYTE buttons[3], buttonsOld[3];
   BOOL buttonsClick[3];
};

struct KEYBOARD {
   BYTE keys[NUM_OF_KEYS], keysOld[NUM_OF_KEYS];
   BOOL keysClick[NUM_OF_KEYS];
};

class WIN {
   HWND hWnd;           // window handle
   INT w, h;            // window size
   HINSTANCE hInstance; // application handle
   BOOL isFullScreen;   // Full Screen flag
   BOOL isActive;       // Active flag
   BOOL isInit;         // Initialization flag
   RECT saveRect;       // Window size save

   friend class RENDER;

protected:
   INT OnCreate(CREATESTRUCT *cs);
   VOID OnDestroy(VOID);
   INT OnEraseBkgnd(HDC hDC);
   VOID OnPaint(VOID);
   virtual VOID Paint(VOID) = 0;
   VOID OnTimer(VOID);
   virtual VOID Timer(VOID) = 0;
   VOID OnSize(INT w, INT h);
   VOID Run(VOID);

   MOUSE mouseData;
   KEYBOARD keyboardData;

   VOID MouseResponce(VOID);
   VOID KeyboardResponce(VOID);

   WIN(VOID);
   static LRESULT CALLBACK WinFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
   const KEYBOARD &GetKeyboardData(VOID);
   const MOUSE &GetMouseData(VOID);
};

#endif // __WIN_H_
