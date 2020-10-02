#include "win.h"
#include "Render.h"
#include "units.h"

WIN::WIN(VOID) : h(0), w(0), 
                 mouseData({ 0 }), keyboardData({ 0 }),
                 isActive(0), isFullScreen(0)
{
   WNDCLASS wc;
   INT e;
   hInstance = GetModuleHandle(NULL);
   isInit = FALSE;

   /* Window class initialization */
   wc.style = CS_VREDRAW | CS_HREDRAW;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hbrBackground = ( HBRUSH )COLOR_WINDOW;
   wc.hCursor = LoadCursor(NULL, IDC_UPARROW);
   wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
   wc.lpszMenuName = NULL;
   wc.hInstance = hInstance;
   wc.lpfnWndProc = WinFunc;
   wc.lpszClassName = "Woof";

   /* Error register handle */
   if (!RegisterClass(&wc)) {
      MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
      return;
   }

   /* Create window */
   hWnd = CreateWindow("Woof", "Feels bad man",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,
      NULL, NULL, hInstance, this);

   if (!hWnd) {
      e = GetLastError();
      MessageBox(NULL, "window was born already dead", "Vse ploxo", MB_OK);
      PostQuitMessage(0x1e);
   }
   isInit = TRUE;
}

LRESULT CALLBACK WIN::WinFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   static WIN *w = NULL;
   CREATESTRUCT *pcs;
   if (w == NULL && msg != WM_CREATE)
      return DefWindowProc(hWnd, msg, wParam, lParam);
   switch (msg) {
   case WM_GETMINMAXINFO:
      (( MINMAXINFO * )lParam)->ptMaxTrackSize.y =
         GetSystemMetrics(SM_CYMAXTRACK) +
         GetSystemMetrics(SM_CYCAPTION) +
         GetSystemMetrics(SM_CYMENU) +
         GetSystemMetrics(SM_CYBORDER) * 2;
      return 0;
   case WM_CREATE:
      pcs = ( CREATESTRUCT * )lParam;
      w = ( WIN * )pcs->lpCreateParams;
      w->hWnd = hWnd;
      w->isInit = TRUE;
      SetTimer(hWnd, 31, 0, NULL);
      return w->OnCreate(( CREATESTRUCT * )lParam) ? 0 : -1;
   case WM_ERASEBKGND:
      return w->OnEraseBkgnd(( HDC )wParam); //(LRESULT)
   case WM_PAINT:
      w->OnPaint();
      return 0;
   case WM_SIZE:
      w->OnSize(LOWORD(lParam), HIWORD(lParam));
      return 0;
   case WM_TIMER:
      w->OnTimer();
      return 0;
   case WM_DESTROY:
   case WM_CLOSE:
      w->OnDestroy();
      return 0;
   default:
      return DefWindowProc(hWnd, msg, wParam, lParam);
   }
}

VOID WIN::Run(VOID)
{
   MSG msg;

   /* Message loop */
   while (TRUE) {
      /* Check message at window message queue */
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
         if (msg.message == WM_QUIT)
            break;
         else {
            /* Displatch message to window */
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
   }
}

VOID WIN::KeyboardResponce(VOID)
{
   GetKeyboardState(keyboardData.keys);

   for (INT i = 0; i < NUM_OF_KEYS; i++) {
      keyboardData.keys[i] >>= 7;
      keyboardData.keysClick[i] = keyboardData.keys[i] && !keyboardData.keysOld[i];
   }
   memcpy(keyboardData.keysOld, keyboardData.keys, NUM_OF_KEYS);
}

VOID WIN::MouseResponce(VOID)
{
   /* Mouse buttons handle */
   BYTE tmpKeys[NUM_OF_KEYS];
   GetKeyboardState(tmpKeys);
   mouseData.buttons[LEFT] = tmpKeys[VK_LBUTTON] >> 7;
   mouseData.buttons[RIGHT] = tmpKeys[VK_RBUTTON] >> 7;
   mouseData.buttons[MIDDLE] = tmpKeys[VK_MBUTTON] >> 7;

   for (INT i = 0; i < 3; i++)
      mouseData.buttonsClick[i] = mouseData.buttons[i] && !mouseData.buttonsOld[i];

   memcpy(mouseData.buttonsOld, mouseData.buttons, 3);

   /* Mouse axis handle */
   POINT pt;

   GetCursorPos(&pt);
   ScreenToClient(hWnd, &pt);
   mouseData.delta.x = pt.x - mouseData.pos.x;
   mouseData.delta.y = pt.y - mouseData.pos.y;
   mouseData.pos.x = pt.x;
   mouseData.pos.y = pt.y;
}

const KEYBOARD &WIN::GetKeyboardData(VOID)
{
   return keyboardData;
}

const MOUSE &WIN::GetMouseData(VOID)
{
   return mouseData;
}

INT WIN::OnCreate(CREATESTRUCT * cs)
{
   /* Mouse init */
   memset(mouseData.buttons, 0, 3 * sizeof(BYTE));
   memset(mouseData.buttonsOld, 0, 3 * sizeof(BYTE));
   memset(mouseData.buttonsClick, 0, 3 * sizeof(BOOL));
   mouseData.delta.x = 0;
   mouseData.delta.y = 0;
   mouseData.pos.x = 0;
   mouseData.pos.y = 0;

   /* Keuboard init */
   memset(keyboardData.keys, 0, NUM_OF_KEYS * sizeof(BYTE));
   memset(keyboardData.keysOld, 0, NUM_OF_KEYS * sizeof(BYTE));
   memset(keyboardData.keysClick, 0, NUM_OF_KEYS * sizeof(BOOL));

   /* Timer init */
   SetTimer(hWnd, 30, 0, NULL);

   return 1;
}

VOID WIN::OnPaint(VOID)
{
   KeyboardResponce();
   MouseResponce();
   Timer();
   Paint();
}

VOID WIN::OnTimer(VOID)
{
   Timer();
   KeyboardResponce();
   MouseResponce();
   PostMessage(hWnd, WM_PAINT, 0, 0);
}

VOID WIN::OnSize(INT width, INT height)
{
   w = width;
   h = height;
}

VOID WIN::OnDestroy(VOID)
{
   PostQuitMessage(0x1e);
}

INT WIN::OnEraseBkgnd(HDC hDC)
{
   return FALSE;
}
