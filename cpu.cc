// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

void
CPU::Start()
{
  PC = emu.mem.ReadWord(0xFFFC);
}

void
CPU::Tick()
{
  uint8_t op = emu.mem.ReadWord(PC++);
}

void
CPU::I00_BRK()
{

}

CPU::IFunPtr CPU::IFunTable[] =
{
  [0x00] = &CPU::I00_BRK,
};
