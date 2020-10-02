#include "anim.h"
#include "unit_menu.h"

#include <list>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInctance, CHAR *cmdLine, INT showCmd)
{
   ANIM anim;

   anim.AddUnit(new UNIT_MENU(&anim, "a.txt.txt"));
   anim.Run();

   return 0x1e;
}
