
#ifndef __MENU_SYSTEM_H_
#define __MENU_SYSTEM_H_

#include "def.h"
#include "mth.h"
#include "units.h"
#include <stdio.h>
#include <vector>
#include <functional>
#pragma warning(disable:4996)

VOID default_func(VOID)
{
}

struct button
{
  VEC2i Pos;
  COLOR Color;
  COLOR SelectedColor;
  char Name[128];
  std::function<VOID(VOID)> OnClick = default_func;
};

struct menu_window
{
  std::vector<button> Buttons;
  VEC2i Pos;
  VEC2i Size;
  COLOR BGColor;
};

class menu_system
{
  private:
  BOOL IsInit = FALSE;
  menu_system( VOID )
  {
  }
  void Clamp(COLOR *c)
  {
    if (c->r > 255 || c->r < 0.0)
      c->r = 255;
    if (c->g > 255 || c->g < 0.0)
      c->g = 255;
    if (c->b > 255 || c->b < 0.0)
      c->b = 255;
  }
  void EvalWinSize(UINT Num)
  {
    menu_window *W = &(MenuWindows[Num]);
    VEC2i max = { 0, 0 };
    for (int i = 0; i < W->Buttons.size(); i++)
    {
      if (max.x < W->Buttons[i].Pos.x)
        max.x = W->Buttons[i].Pos.x;
      if (max.y < W->Buttons[i].Pos.y)
        max.y = W->Buttons[i].Pos.y;
    }
    max.x += 10;
    max.y += 10;

    W->Size = max + ButtonSize;
}

  public:
  const VEC2i ButtonSize = { 102, 30 };
  const VEC2i BorderSize = { 5, 5 };
  std::vector<menu_window> MenuWindows;
  UINT SelectedButton;
  UINT SelectedWindow;

  std::function<VOID(VOID)> &SetCallbackByName(std::string Str)
  {
     for (auto &w : MenuWindows)
        for (auto &b : w.Buttons)
           if (Str == std::string(b.Name))
              return b.OnClick;
     throw "ERROR";
  }

  menu_system(std::string Filename)
  {
    FILE *F;
    char Buf[128];
    int tmp;

    if (!(F = fopen(Filename.c_str(), "r")))
    {
      MessageBox(NULL, "Invalid file name", "Error", NULL);
      throw "Error";
    }

    UINT CurWindow = 0;
    while (!feof(F))
    {
      fscanf(F, "%s", Buf);
      if (!strcmp("NumOfWindows", Buf))
      {  
        fscanf(F, "%i", &tmp);
        MenuWindows.resize(tmp);
      }
      else if (!strcmp("WindowBegin", Buf))
      {
        fscanf(F, "%s", Buf);
        if (!strcmp("Position", Buf))
          fscanf(F, "%i %i", &(MenuWindows[CurWindow].Pos.x), &(MenuWindows[CurWindow].Pos.y));

        fscanf(F, "%s", Buf);
        if (!strcmp("Color", Buf))
        {
          fscanf(F, "%hhu %hhu %hhu", &MenuWindows[CurWindow].BGColor.r,
                                &MenuWindows[CurWindow].BGColor.g,
                                &MenuWindows[CurWindow].BGColor.b);
          Clamp(&MenuWindows[CurWindow].BGColor);
      }
      while (strcmp("WindowEnd", Buf) && !feof(F))
      {
        fscanf(F, "%s", Buf);
        if (!strcmp("NumOfButtons", Buf))
        {
          fscanf(F, "%i", &tmp);
          if (tmp == 0)
            MessageBox(NULL, "Cant create window without buttons", "Error", NULL);

        }
        else if (!strcmp("ButtonBegin", Buf))
        {
          button CurrentButton;
          fscanf(F, "%s", Buf);
          if (!strcmp("Position", Buf))
            fscanf(F, "%i %i", &CurrentButton.Pos.x, &CurrentButton.Pos.y);

          fscanf(F, "%s", Buf);
          if (!strcmp("Color", Buf))
            fscanf(F, "%hhu %hhu %hhu", &CurrentButton.Color.r, &CurrentButton.Color.g, &CurrentButton.Color.b);

          fscanf(F, "%s", Buf);
          if (!strcmp("SelectedColor", Buf))
            fscanf(F, "%hhu %hhu %hhu", &CurrentButton.SelectedColor.r, &CurrentButton.SelectedColor.g, &CurrentButton.SelectedColor.b);

          fscanf(F, "%s", Buf);
          if (!strcmp("Name", Buf))
            fscanf(F, "%s", CurrentButton.Name);

          fscanf(F, "%s", Buf);
          MenuWindows[CurWindow].Buttons.push_back(CurrentButton);
        }
      }
      CurWindow++;
    }
  }

  for (int i = 0; i < MenuWindows.size(); i++)
    EvalWinSize(i);

  SelectedWindow = 0;
  SelectedButton = 0;

  IsInit = TRUE;
  }
};

struct UNIT_MENU : public UNIT {
   ANIM *ani;
   menu_system Menu;
   UNIT_MENU(ANIM * anim, std::string filename) : ani(anim), Menu(filename)
   {
      Menu.SetCallbackByName("cgsg") = []()
      {
         MessageBox(NULL, "FOREVER", "OK", NULL);
      };
   }

   virtual VOID Draw(VOID)
   {
      int q = 0;
      for (auto &win : Menu.MenuWindows)
      {
         ani->Rect(win.Pos.x, win.Pos.y, win.Pos.x + win.Size.x, win.Pos.y + win.Size.y, win.BGColor);
         
         for (int i = 0; i < win.Buttons.size(); i++)
            if (i == Menu.SelectedButton && q == Menu.SelectedWindow)
               ani->Rect(win.Pos.x + win.Buttons[i].Pos.x + Menu.BorderSize.x,
                         win.Pos.y + win.Buttons[i].Pos.y + Menu.BorderSize.y,
                         win.Pos.x + win.Buttons[i].Pos.x + Menu.BorderSize.x + Menu.ButtonSize.x,
                         win.Pos.y + win.Buttons[i].Pos.y + Menu.BorderSize.y + Menu.ButtonSize.y,
                         win.Buttons[i].SelectedColor);
            else
               ani->Rect(win.Pos.x + win.Buttons[i].Pos.x + Menu.BorderSize.x,
                         win.Pos.y + win.Buttons[i].Pos.y + Menu.BorderSize.y,
                         win.Pos.x + win.Buttons[i].Pos.x + Menu.BorderSize.x + Menu.ButtonSize.x,
                         win.Pos.y + win.Buttons[i].Pos.y + Menu.BorderSize.y + Menu.ButtonSize.y,
                         win.Buttons[i].Color);
         q++;
      }
   }

   virtual VOID Response(VOID)
   {
     if (ani->GetKeyboardData().keysClick['E'])    //VK_RETURN
       Menu.MenuWindows[Menu.SelectedWindow].Buttons[Menu.SelectedButton].OnClick();
     else if (ani->GetKeyboardData().keysClick[VK_DOWN] && Menu.SelectedButton < Menu.MenuWindows[Menu.SelectedWindow].Buttons.size() - 1)
       Menu.SelectedButton++;
     else if (ani->GetKeyboardData().keysClick[VK_UP] && Menu.SelectedButton > 0)
       Menu.SelectedButton--;
     else if (ani->GetKeyboardData().keysClick[VK_RIGHT] && Menu.SelectedWindow < Menu.MenuWindows.size() - 1)
     {
       Menu.SelectedWindow++;
       Menu.SelectedButton = 0;
     }
     else if (ani->GetKeyboardData().keysClick[VK_LEFT] && Menu.SelectedWindow > 0)
     {
       Menu.SelectedWindow--;
       Menu.SelectedButton = 0;
       //MessageBox(NULL, "Callback has come because gavno is here", "It works", NULL);
     }
   }
};

#endif // __MENU_SYSTEM_H_