// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __JIT_H__
#define __JIT_H__

/**
 * x64 Code Emitter
 */
class JIT
{
public:
  /**
   * Creates a new JIT instance
   */
  JIT(Emulator &emu);

  /**
   * Destroys the JIT instance, freeing segments
   */
  ~JIT();

  /**
   * Executes the compiled segment
   */
  void Execute();

private:
  /**
   * Allocates memory and marks it as writeable
   */
  void Create();

  /**
   * Frees memory allocated by the JIT
   */
  void Destroy();

private:
  /// Reference to the CPU
  CPU &cpu;
  /// Reference to memory
  Memory &mem;
  /// Start address of the segment
  uint16_t addr;
  /// Bytes this page can hold
  size_t capacity;
  /// Bytes present in the page
  size_t size;
  /// 6502 cycles required to execute
  size_t cycles;
  /// Pointer to executable memory
  uint8_t *prg;
};

#endif
