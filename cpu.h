// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __CPU_H__
#define __CPU_H__

#define ROWL(r, I0, I1, I2, I3, I4, I5, I6, I7)\
  void I##r##0_##I0(); void I##r##1_##I1();\
  void I##r##2_##I2(); void I##r##3_##I3();\
  void I##r##4_##I4(); void I##r##5_##I5();\
  void I##r##6_##I6(); void I##r##7_##I7();

#define ROWH(r, I0, I1, I2, I3, I4, I5, I6, I7)\
  void I##r##8_##I0(); void I##r##9_##I1();\
  void I##r##A_##I2(); void I##r##B_##I3();\
  void I##r##C_##I4(); void I##r##D_##I5();\
  void I##r##E_##I6(); void I##r##F_##I7();

/**
 * Interpretative emulation of the Ricoh 2A03 CPU found in the NES
 */
class CPU
{
public:
  CPU(Emulator &emu)
    : emu(emu)
  {
  }

  /**
   * Executes the reset interrupt
   */
  void Start();

  /**
   * Executes a single instruction
   */
  void Tick();

private:

  /**
   * Pushes a word onto the stack and decrements the stack pointer
   * @param word 16 bit value to be placed onto the stack
   */
  void PushWord(uint16_t word);

  /**
   * Pops a word from the stack and increments the stack pointer
   * @returns 16 bit value popped from the stack
   */
  uint16_t PopWord();

private:

  ROWL(0, BRK, ORA, KIL, SLO, DOP, ORA, ASL, SLO)
  ROWH(0, PHP, ORA, ASL, AAC, TOP, ORA, ASL, SLO)
  ROWL(1, BPL, ORA, KIL, SLO, DOP, ORA, ASL, SLO)
  ROWH(1, CLC, ORA, NOP, SLO, TOP, ORA, ASL, SLO)
  ROWL(2, JSR, AND, KIL, RLA, BIT, AND, ROL, RLA)
  ROWH(2, PLP, AND, ROL, AAC, BIT, AND, ROL, RLA)
  ROWL(3, BMI, AND, KIL, RLA, DOP, AND, ROL, RLA)
  ROWH(3, SEC, AND, NOP, RLA, TOP, AND, ROL, RLA)
  ROWL(4, RTI, EOR, KIL, SRE, DOP, EOR, LSR, SRE)
  ROWH(4, PHA, EOR, LSR, ASR, JMP, EOR, LSR, SRE)
  ROWL(5, BVC, EOR, KIL, SRE, DOP, EOR, LSR, SRE)
  ROWH(5, CLI, EOR, NOP, SRE, TOP, EOR, LSR, SRE)
  ROWL(6, RTS, ADC, KIL, RRA, DOP, ADC, ROR, RRA)
  ROWH(6, PLA, ADC, ROR, ARR, JMP, ADC, ROR, RRA)
  ROWL(7, BVS, ADC, KIL, RRA, DOP, ADC, ROR, RRA)
  ROWH(7, SEI, ADC, NOP, RRA, TOP, ADC, ROR, RRA)
  ROWL(8, DOP, STA, DOP, AAX, STY, STA, STX, AAX)
  ROWH(8, DEY, DOP, TXA, XAA, STY, STA, STX, AAX)
  ROWL(9, BCC, STA, KIL, AXA, STY, STA, STX, AAX)
  ROWH(9, TYA, STA, TXS, XAS, SYA, STA, SXA, AXA)
  ROWL(A, LDY, LDA, LDX, LAX, LDY, LDA, LDX, LAX)
  ROWH(A, TAY, LDA, TAX, ATX, LDY, LDA, LDX, LAX)
  ROWL(B, BCS, LDA, KIL, LAX, LDY, LDA, LDX, LAX)
  ROWH(B, CLV, LDA, TSX, LAR, LDY, LDA, LDX, LAX)
  ROWL(C, CPY, CMP, DOP, DCP, CPY, CMP, DEC, DCP)
  ROWH(C, INY, CMP, DEX, AXS, CPY, CMP, DEC, DCP)
  ROWL(D, BNE, CMP, KIL, DCP, DOP, CMP, DEC, DCP)
  ROWH(D, CLD, CMP, NOP, DCP, TOP, CMP, DEC, DCP)
  ROWL(E, CPX, SBC, DOP, ISC, CPX, SBC, INC, ISC)
  ROWH(E, INX, SBC, NOP, SBC, CPX, SBC, INC, ISC)
  ROWL(F, BEQ, SBC, KIL, ISC, DOP, SBC, INC, ISC)
  ROWH(F, SED, SBC, NOP, ISC, TOP, SBC, INC, ISC)

  // Dispatch table
  typedef void (CPU::*IFunPtr) ();
  static IFunPtr IFunTable[0x100];

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
      // Carry flag
      uint8_t C : 1;
      // Zero flag
      uint8_t Z : 1;
      // Interrupt priority level
      uint8_t I : 1;
      // BCD flag
      uint8_t D : 1;
      // Only exists on the stack
      uint8_t   : 1;
      // Always 1 if pushed
      uint8_t   : 1;
      // Overflow flag
      uint8_t V : 1;
      // Negative flag (bit 7)
      uint8_t N : 1;
    };

    uint8_t P;
  };

  /// Emulator reference
  Emulator &emu;
};

#endif /*__CPU_H__*/
