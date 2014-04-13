// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __NES_H__
#define __NES_H__

#include <stdint.h>

class Emulator;

#include "cpu.h"
#include "mem.h"
#include "apu.h"
#include "ppu.h"

/**
 * NES Emulator
 */
class Emulator
{
public:
  /// Initialises the emulator
  Emulator()
    : cpu(this)
    , memory(this)
    , ppu(this)
    , apu(this)
  {
  }

  /// Cleanup code
  ~Emulator()
  {
  }

  /// Runs the emulator
  void Run();

private:
  /// Central Processing Unit
  CPU    cpu;
  /// Main memory & ROM
  Memory memory;
  /// Picture Processing Unit
  PPU    ppu;
  /// Audio Processing Unit
  APU    apu;

  // Allow access between submodules
  friend class CPU;
  friend class PPU;
  friend class APU;
  friend class Memory;
};

#endif /*__NES_H__*/
