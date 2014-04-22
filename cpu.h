// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __CPU_H__
#define __CPU_H__

#define ROWL(r, a, b, c, d, e, f, g, h)\
  I(r,0,a) I(r,1,b) I(r,2,c) I(r,3,d) I(r,4,e) I(r,5,f) I(r,6,g) I(r,7,h)

#define ROWH(r, a, b, c, d, e, f, g, h)\
  I(r,8,a) I(r,9,b) I(r,A,c) I(r,B,d) I(r,C,e) I(r,D,f) I(r,E,g) I(r,F,h)

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

  /**
   * Prints the state of the CPU
   */
  void Dump(std::ostream &os);

private:
  /**
   * List of addressing modes
   */
  enum AddrMode
  {
    NOP,
    REG_A,
    REG_X,
    REG_Y,
    REG_S,
    IMM,
    MEM_ZP,
    MEM_ZP_X,
    MEM_ZP_Y,
    MEM_ABS,
    MEM_ABS_X,
    MEM_ABS_Y,
    MEM_IDX_X,
    MEM_IDX_Y,
    MEM_IND_X,
    MEM_IND_Y
  };

  /**
   * Returns the value of the P register by or-ing the values of the flags
   */
  uint8_t GetP();

  /**
   * Reads the values of flags from the P register
   */
  void SetP(uint8_t P);

  /**
   * Pushes a word onto the stack, decrementing S by 2
   */
  void PushWord(uint16_t word);

  /**
   * Pushes a byte onto the stack, decrementing S by 1
   */
  void PushByte(uint8_t byte);

  /**
   * Pops a word from the stack, incrementing S by 2
   */
  uint16_t PopWord();

  /**
   * Pops a byte from the stack, incrementing S by 1
   */
  uint8_t PopByte();

  /**
   * Performs an instruction, reading in an operand and modifying it
   */
  typedef void (CPU::*InstrFunc) (uint8_t &M);

  /**
   * Wraps functions that read & write
   */
  template <AddrMode ReadMode, AddrMode WriteMode = ReadMode>
  inline void Instr(InstrFunc func);

private:
  void AAX(uint8_t &M);
  void ADC(uint8_t &M);
  void AND(uint8_t &M);
  void ASL(uint8_t &M);
  void AXA(uint8_t &M);
  void Bcc(uint8_t  M);
  void BIT(uint8_t &M);
  void CMP(uint8_t &M);
  void CPX(uint8_t &M);
  void CPY(uint8_t &M);
  void DCP(uint8_t &M);
  void DEC(uint8_t &M);
  void EOR(uint8_t &M);
  void INC(uint8_t &M);
  void ISC(uint8_t &M);
  void LAR(uint8_t &M);
  void LAX(uint8_t &M);
  void LDM(uint8_t &M);
  void LSR(uint8_t &M);
  void ORA(uint8_t &M);
  void RLA(uint8_t &M);
  void ROL(uint8_t &M);
  void ROR(uint8_t &M);
  void RRA(uint8_t &M);
  void SBC(uint8_t &M);
  void SLO(uint8_t &M);
  void SRE(uint8_t &M);
  void STM(uint8_t &M);

private:
  #define I(r, c, i) void I##r##c##_##i();
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
  #undef I

  // Dispatch table
  typedef void (CPU::*IFunPtr) ();
  static IFunPtr IFunTable[0x100];

private:
  // Program counter
  uint16_t PC;
  // Accumulator
  uint8_t A;
  // X index
  uint8_t X;
  // Y index
  uint8_t Y;
  // Stack pointer
  uint8_t S;
  // P0: Carry flag
  uint8_t C;
  // P1: Zero flag
  uint8_t Z;
  // P2: Interrupt priority level
  uint8_t I;
  // P3: BCD flag
  uint8_t D;
  // P4: Overflow flag
  uint8_t V;
  // P5: Negative flag
  uint8_t N;
  /// Emulator reference
  Emulator &emu;
  /// Cycle counter
  uint64_t cycles;
  /// JIT needs access to all these things
  friend class JIT;
};

#endif /*__CPU_H__*/
