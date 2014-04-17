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

private:

  /**
   * Pushes a word onto the stack and decrements the stack pointer
   * @param word 16 bit value to be placed onto the stack
   */
  void PushWord(uint16_t word);

  /**
   * Pushes a byte on the stack, decrementing the stack pointer
   */
  void PushByte(uint8_t byte);

  /**
   * Pops a word from the stack and increments the stack pointer
   * @returns 16 bit value popped from the stack
   */
  uint16_t PopWord();

  /**
   * Pops a byte from the stack, incrementing the stack pointer
   */
  uint8_t PopByte();

private:

  /**
   * Moves data between registers or memory
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool c, uint8_t),
           bool flags = true>
  inline void Move()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);
    if (flags)
    {
      Z = M == 0;
      N = M & 0x80 ? 1 : 0;
    }

    (this->*Write)(addr, c, M);
  }

  /**
   * Conditional branch
   */
  inline void Branch(bool branch)
  {
    PC += branch ? (int8_t)ReadImmediate() : 1;
  }

  /**
   * Bitwise or
   */
  template<typename Op, uint8_t (CPU::*Read)(uint16_t &, bool &)>
  inline void Bitwise()
  {
    uint16_t addr;
    uint8_t M;
    bool c;
    Op op;

    M = (this->*Read)(addr, c);
    A = op(A, M);
    Z = A == 0;
    N = A & 0x80 ? 1 : 0;
  }

  /**
   * Bit test
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &)>
  inline void BIT()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);
    Z = (A & M) == 0;
    V = M & 0x40 ? 1 : 0;
    N = M & 0x80 ? 1 : 0;
  }

  /**
   * Rotate Right
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool c, uint8_t)>
  inline void ROR()
  {
    uint8_t M, newCarry;
    uint16_t addr;
    bool c;

    M = (this->*Read)(addr, c);

    newCarry = M & 0x1;
    M = (C << 7) | (M >> 1);
    C = newCarry;
    Z = M == 0;
    N = M & 0x80 ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Rotate Left
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool c, uint8_t)>
  inline void ROL()
  {
    uint8_t M, newCarry;
    uint16_t addr;
    bool c;

    M = (this->*Read)(addr, c);

    newCarry = M & 0x80;
    M = C | (M << 1);
    C = newCarry ? 1 : 0;
    Z = M == 0;
    N = M & 0x80 ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Arithmetic shift left
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool c, uint8_t)>
  inline void ASL()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);

    C = M & 0x80 ? 1 : 0;
    M <<= 1;
    Z = M == 0;
    N = M & 0x80 ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Logical shift right
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool c, uint8_t)>
  inline void LSR()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);

    C = M & 0x01;
    M >>= 1;
    Z = M == 0;
    N = 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Add with carry
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &)>
  inline void ADC()
  {
    uint16_t addr, r, M;
    bool c;

    M = (this->*Read)(addr, c);
    r = M + A + C;

    C = r & 0xFF00 ? 1 : 0;
    Z = (r & 0xFF) == 0;
    N = r & 0x80 ? 1 : 0;
    V = (A & 0x80) == (M & 0x80) && (A & 0x80) != (r & 0x80);

    A = r;
  }

  /**
   * Subtract with carry
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &)>
  inline void SBC()
  {
    uint16_t r, M, addr;
    bool c;

    M = (this->*Read)(addr, c);
    r = A - M - !C;

    C = !(r & 0xFF00);
    Z = (r & 0xFF) == 0;
    N = (r & 0x80) ? 1 : 0;
    V = (A & 0x80) != (M & 0x80) && (M & 0x80) == (r & 0x80);

    A = r;
  }

  /**
   * Compare
   */
  template<uint8_t (CPU::*ReadR)(uint16_t &, bool &),
           uint8_t (CPU::*ReadM)(uint16_t &, bool &)>
  inline void CMP()
  {
    uint16_t addr;
    uint8_t M, R;
    bool c;

    M = (this->*ReadM)(addr, c);
    R = (this->*ReadR)(addr, c);

    Z = M == R;
    C = R >= M;
    N = ((R - M) & 0x80) ? 1 : 0;
  }

  /**
   * Increment or decrement by a constant
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t),
           int8_t V>
  inline void IncDec()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);
    M += V;
    Z = M == 0;
    N = M & 0x80 ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Load A & X from memory
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &)>
  inline void LAX()
  {
    uint16_t addr;
    bool c;

    A = X = (this->*Read)(addr, c);
    Z = A == 0;
    N = A & 0x80 ? 1 : 0;
  }

  /**
   * M = A & X
   */
  template<void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void AAX()
  {
    (this->*Write)(0, false, A & X);
  }

  template<void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void AXA()
  {
    (this->*Write)(0, false, A & X & 0x7);
  }

  /**
   * Subtract 1 from memory without borrow
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void DCP()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c) - 1;

    Z = M == A;
    C = A >= M;
    N = ((A - M) & 0x80) ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Increase memory by one & subtract from accumulator
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void ISC()
  {
    uint16_t r, addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c) + 1;
    r = A - M - !C;

    C = !(r & 0xFF00);
    Z = (r & 0xFF) == 0;
    N = (r & 0x80) ? 1 : 0;
    V = (A & 0x80) != (M & 0x80) && (M & 0x80) == (r & 0x80);

    (this->*Write)(addr, c, M);
    A = r;
  }

  /**
   * M <<= 1, A = A | M
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void SLO()
  {
    uint16_t r, addr;
    uint8_t M;
    bool c;

    M = (this->*Read)(addr, c);
    C = M & 0x80 ? 1 : 0;
    M <<= 1;

    A = A | M;
    Z = A == 0;
    N = (A & 0x80) ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Rotate M left, A = A & M
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void RLA()
  {
    uint16_t r, addr;
    uint8_t M, newCarry;
    bool c;

    M = (this->*Read)(addr, c);

    newCarry = M & 0x80 ? 1 : 0;
    M = (M << 1) | C;
    C = newCarry;

    A = A & M;
    Z = A == 0;
    N = (A & 0x80) ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Shift M right, A = A ^ M
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void SRE()
  {
    uint16_t addr;
    uint8_t M, newCarry;
    bool c;

    M = (this->*Read)(addr, c);
    C = M & 0x01;
    M >>= 1;

    A = A ^ M;
    Z = A == 0;
    N = (A & 0x80) ? 1 : 0;

    (this->*Write)(addr, c, M);
  }

  /**
   * Rotate M right, A = A + M
   */
  template<uint8_t (CPU::*Read)(uint16_t &, bool &),
           void (CPU::*Write)(uint16_t, bool, uint8_t)>
  inline void RRA()
  {
    uint16_t r, addr;
    uint8_t M, newCarry;
    bool c;

    M = (this->*Read)(addr, c);
    newCarry = M & 0x01;
    M = (M >> 1) | (C << 7);
    (this->*Write)(addr, c, M);

    r = M + A + newCarry;

    C = r & 0xFF00 ? 1 : 0;
    Z = (r & 0xFF) == 0;
    N = r & 0x80 ? 1 : 0;
    V = (A & 0x80) == (M & 0x80) && (A & 0x80) != (r & 0x80);

    A = r;
  }

private:

  uint8_t ReadImmediate();
  uint8_t ReadImmediate(uint16_t &addr, bool &c);
  uint8_t ReadZeroPage(uint16_t &addr, bool &c);
  uint8_t ReadZeroPageX(uint16_t &addr, bool &c);
  uint8_t ReadZeroPageY(uint16_t &addr, bool &c);
  uint8_t ReadAbsolute(uint16_t &addr, bool &c);
  uint8_t ReadAbsoluteX(uint16_t &addr, bool &c);
  uint8_t ReadAbsoluteY(uint16_t &addr, bool &c);
  uint8_t ReadIndexedIndirectX(uint16_t &addr, bool &c);
  uint8_t ReadIndexedIndirectY(uint16_t &addr, bool &c);
  uint8_t ReadIndirectIndexedX(uint16_t &addr, bool &c);
  uint8_t ReadIndirectIndexedY(uint16_t &addr, bool &c);

  inline uint8_t ReadA(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return A;
  }

  inline uint8_t ReadX(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return X;
  }

  inline uint8_t ReadY(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return Y;
  }

  inline uint8_t ReadS(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return S;
  }

private:

  void WriteA(uint16_t addr, bool c, uint8_t v) { A = v; }
  void WriteX(uint16_t addr, bool c, uint8_t v) { X = v; }
  void WriteY(uint16_t addr, bool c, uint8_t v) { Y = v; }
  void WriteS(uint16_t addr, bool c, uint8_t v) { S = v; }
  void WriteZeroPage(uint16_t addr, bool c, uint8_t v);
  void WriteZeroPageX(uint16_t addr, bool c, uint8_t v);
  void WriteZeroPageY(uint16_t addr, bool c, uint8_t v);
  void WriteAbsolute(uint16_t addr, bool c, uint8_t v);
  void WriteAbsoluteX(uint16_t addr, bool c, uint8_t v);
  void WriteAbsoluteY(uint16_t addr, bool c, uint8_t v);
  void WriteIndexedIndirectX(uint16_t addr, bool c, uint8_t v);
  void WriteIndexedIndirectY(uint16_t addr, bool c, uint8_t v);
  void WriteIndirectIndexedX(uint16_t addr, bool c, uint8_t v);
  void WriteIndirectIndexedY(uint16_t addr, bool c, uint8_t v);

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
  struct
  {
    // Carry flag
    uint8_t C;
    // Zero flag
    uint8_t Z;
    // Interrupt priority level
    uint8_t I;
    // BCD flag
    uint8_t D;
    // Overflow flag
    uint8_t V;
    // Negative flag (bit 7)
    uint8_t N;
  };

  /// Emulator reference
  Emulator &emu;
};

#endif /*__CPU_H__*/
