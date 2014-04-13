// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __CPU_H__
#define __CPU_H__

/**
 * Interpretative emulation of the Ricoh 2A03 CPU found in the NES
 */
class CPU
{
public:
  CPU(Emulator *emu)
  {
  }
private:
  // Accumulator
  uint8_t A;

  // X index
  uint8_t X;

  // Y index
  uint8_t Y;

  // Stack pointer
  uint8_t S;

  // Program counter
  uint16_t PC;

  // Processor flags
  union
  {
    struct
    {
      // Negative flag (bit 7)
      uint8_t N : 1;
      // Overflow flag
      uint8_t V : 1;
      // Always 1 if pushed
      uint8_t   : 1;
      // Only exists on the stack
      uint8_t B : 1;
      // BCD flag
      uint8_t D : 1;
      // Interrupt priority level
      uint8_t I : 1;
      // Zero flag
      uint8_t Z : 1;
      // Carry flag
      uint8_t C : 1;
    };

    uint8_t P;
  };
};

#endif /*__CPU_H__*/
