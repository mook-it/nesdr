// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

// -----------------------------------------------------------------------------
// MMC0
// -----------------------------------------------------------------------------
MMC0::MMC0(Memory &mem, INes &cart)
  : Memory::Mapper(mem, cart)
{
  mem.LoadBank(0, Memory::LROM);
  mem.LoadBank(1, Memory::HROM);
}

void
MMC0::Write(uint16_t addr, uint8_t value)
{
}

// -----------------------------------------------------------------------------
// MMC1
// -----------------------------------------------------------------------------
MMC1::MMC1(Memory &mem, INes &cart)
  : Memory::Mapper(mem, cart)
  , CHR0(0)
  , CHR1(0)
  , PRG(0)
  , M(0)
  , W(0)
  , C(0)
  , P(1)
  , S(1)
  , index(0)
  , temp(0)
{
  mem.LoadBank(0, Memory::LROM);
  mem.LoadBank(1, Memory::HROM);
}

void
MMC1::Write(uint16_t addr, uint8_t value)
{
  if (value & 0x80)
  {
    temp = 0x00;
    index = 0;
    P = S = 1;
  }
  else
  {
    temp = temp | ((value & 0x01) << index);
    if (++index >= 5)
    {
      switch (addr & 0xF000)
      {
        case 0x8000: case 0x9000:
        {
          C = temp & 0x10 ? 1 : 0;
          P = temp & 0x08 ? 1 : 0;
          S = temp & 0x04 ? 1 : 0;
          M = temp & 0x03;
          UpdatePRG();
          UpdateCHR();
          break;
        }
        case 0xA000: case 0xB000:
        {
          CHR0 = temp;
          UpdateCHR();
          break;
        }
        case 0xC000: case 0xD000:
        {
          CHR1 = temp;
          UpdateCHR();
          break;
        }
        case 0xE000: case 0xF000:
        {
          W = temp & 0x10 ? 1 : 0;
          PRG = temp & 0x0F;
          UpdatePRG();
          break;
        }
      }

      index = 0;
      temp = 0x00;
    }
  }
}

void
MMC1::UpdatePRG()
{
  if (P == 0)
  {
    mem.LoadBank(PRG, Memory::LROM);
    mem.LoadBank(PRG + 1, Memory::HROM);
  }
  else
  {
    if (S == 0)
    {
      mem.LoadBank(0, Memory::LROM);
      mem.LoadBank(PRG, Memory::HROM);
    }
    else
    {
      mem.LoadBank(PRG, Memory::LROM);
      mem.LoadBank(0x0F, Memory::HROM);
    }
  }
}

void
MMC1::UpdateCHR()
{

}
