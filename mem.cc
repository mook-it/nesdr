// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

void
Memory::LoadINes(const std::string& nes)
{
  cart.reset(new INes(nes));
  if (emu.verbose)
  {
    cart->Dump(std::cout);
  }
}

void
Memory::LoadBank(uint16_t bank, const ROMArea &where)
{
  const std::vector<uint8_t>& rom = cart->GetPRGRom();
  if (bank << 14 > rom.size())
  {
    std::stringstream ss;
    ss << "Bank index #" << bank << " out of range";
    throw std::runtime_error(ss.str());
  }

  memcpy((where == LROM) ? lrom : hrom, &rom[bank << 14], 0x4000);
}

uint8_t
Memory::ReadByte(uint16_t addr)
{
  // First bank of Game RAM - this path is most likely to be
  // taken as the stack & most runtime variables reside here
  if (__builtin_expect((addr & 0xF000) == 0x0000, 1))
  {
    return memory[addr];
  }

  // Second most likely path as it contains read-only data or
  // the SRAM which is not modified too often. Testing for
  // this branch is also cheap
  if (addr >= 0x4000)
  {
    return memory[addr];
  }

  // Written during VBlank periods, when naive emulation
  // is usually used, so we don't care much about the cost
  // of this branch. Also, there registers are written
  // more ofthen than read
  if ((addr & 0xC000) == 0x0000)
  {
    return rppu[addr & 0x0007];
  }

  // Least likely path - mirrored ram that must
  // be remapped to the first 2Kb of memory
  return ram[addr & 0x07FF];
}

void
Memory::WriteByte(uint16_t addr, uint8_t byte)
{
  // First bank, most likely to be accessed
  if (__builtin_expect((addr & 0xF000) == 0x0000, 1))
  {
    memory[addr] = byte;
    return;
  }

  throw std::runtime_error("Unimplemented");
}
