// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

uint8_t
Memory::ReadByte(uint16_t addr)
{
  if (__builtin_expect((addr & 0xD000) == 0x0000, 1))
  {
    // RAM & mirror images
    return ram[addr & 0x07FF];
  }
  else if ((addr & 0xC000) == 0x0000)
  {
    // PPU control registers
    // ppu->Read(addr & 0x0007);
  }
  else if ((addr & 0xFFD0) == 0x4020)
  {
    // APU registers
    // apu->Read(addr & 0x001F);
  }
  else if ((addr & 0xD000) == 0x4000)
  {
    // Cartridge expansion ROM
    return erom[addr - 0x4000];
  }
  else
  {
    // SRAM, low ROM, high ROM
    return hmem[addr - 0x6000];
  }
}

void
Memory::WriteByte(uint16_t addr, uint8_t byte)
{

}
