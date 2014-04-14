// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __NES_H__
#define __NES_H__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <getopt.h>

class Emulator;

#include "ines.h"
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
  /**
   * Initialises the emulator
   */
  Emulator()
    : cpu(*this)
    , mem(*this)
    , ppu(*this)
    , apu(*this)
    , debug(0)
    , verbose(0)
  {
  }

  /**
   * Destroys the emulator
   */
  ~Emulator()
  {
    Destroy();
  }

  /**
   * Starts the emulator
   */
  void Main(int argc, char **argv);

private:
  /**
   * Initialises SDL & start emulation
   */
  void Init(const std::string& file);

  /**
   * Main emulation loop
   */
  void Run();

  /**
   * Destroys SDL & cleans up
   */
  void Destroy();

  /**
   * Prints some help
   */
  void Usage(std::ostream& os);

private:
  /// Central Processing Unit
  CPU cpu;
  /// Main memory & ROM
  Memory mem;
  /// Picture Processing Unit
  PPU ppu;
  /// Audio Processing Unit
  APU apu;
  /// Debug flag
  int debug;
  /// Verbose flag
  int verbose;

  // Allow access between submodules
  friend class CPU;
  friend class PPU;
  friend class APU;
  friend class Memory;
};

#endif /*__NES_H__*/
