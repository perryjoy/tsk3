#pragma once
#include "win.h"
#include "render.h"
#include "units.h"

class ANIM : public virtual WIN, public virtual RENDER {
   std::vector<UNIT *> units;

public:
   ANIM(VOID) : WIN(), RENDER(this)
   {
   }

   ~ANIM(VOID)
   {
      for (auto &i : units) {
         delete i;
      }
   }

   VOID AddUnit(UNIT *uni)
   {
      uni->ani = this;
      units.push_back(uni);
   }

   virtual VOID Paint(VOID)
   {
      RENDER::StartFrame();
      for (auto &i : units)
         i->Draw();
      RENDER::EndFrame();
   }
   virtual VOID Timer(VOID)
   {
      for (auto &i : units)
         i->Response();
   }

   VOID Run(VOID)
   {
      WIN::Run();
   }
};